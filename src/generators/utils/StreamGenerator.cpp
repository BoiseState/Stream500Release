// Copyright (c) 2018 Boise State University 
// All rights reserved.

// Developed by:      Stream500 Team
//                     Boise State University
//                     https://github.com/BoiseState/Stream500

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal with
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// Redistributions of source code must retain the above copyright notice, this list
// of conditions and the following disclaimers.  Redistributions in binary form
// must reproduce the above copyright notice, this list of conditions and the
// following disclaimers in the documentation and/or other materials provided with
// the distribution.  Neither the names of Stream500 Team, Boise State University,
// nor the names of its contributors may be used to endorse or promote products 
// derived from this Software without specific prior written permission. 

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE CONTRIBUTORS
// OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE SOFTWARE.

#include "StreamGenerator.h"

/******************************************************************************
 * Parameters:
 * init_cp - The Configuration object that stores all parameters for a run.
 * Goal:
 * Setup all buffers, random generators, variables, etc. associated with
 * generating a graph stream.
*******************************************************************************/
StreamGenerator::StreamGenerator(ConfigParser* init_cp)
{
  cp = init_cp;
  if (cp->get_rand_seed() > 0)
  {
    rand_generator.seed(cp->get_rand_seed());
  }
  else
  {
    rand_generator.seed(device());
  }

  prob_distribution = std::uniform_real_distribution<double>(0.0,1.0);
  length_distribution = std::uniform_int_distribution<int>(cp->get_min_len(), cp->get_max_len());

  msg_buf = malloc(cp->get_max_msg_len());
  query_datum = (query *)malloc(sizeof(query));
  edge_datum = (edge *)malloc(sizeof(edge));
  path;
  path = (edge **)malloc(cp->get_max_len() * sizeof(edge *));
  for (int i = 0; i < cp->get_max_len(); i++)
  {
    path[i] = (edge *)malloc(sizeof(edge));
  }

  num_edges = (int64_t)cp->get_edge_factor() << cp->get_log_num_nodes();
  rate = cp->get_gen_rate();
  prev_edge_count = 0;

  results_buf = new ResultsBuffer(cp->get_result_buf_size());
  thread_vars = (thread_struct*)malloc(sizeof(thread_struct));
  setup_thread_struct(thread_vars, results_buf, &correct_query_count, &wrong_yes_query_count,
                        &wrong_no_query_count, &edges_rcvd_count, &packets_rcvd_count);

//  wait_until_tcp_server_is_ready(cp->get_to_gen_port());
  int status = pthread_create(&listen_thread, NULL, listen_to_socket, (void*)thread_vars);
  if (status != 0)
  {
    perror("Listen thread create failed!");
    exit(1);
  }
}

/******************************************************************************
 * Parameters:
 * generator - The Generator Object used for generating edges of a graph
 * obfuscator - The Obfuscator Object used for obfuscating edges/queries of a graph
 * sender - The Sender Object used for sending edges/queries of a graph
 * time_start - The time at which the stream began (in seconds) 
 * Goal:
 * Using the generator, obfuscator, and sender create a stream of edges and queries
 * that are sent to a Responder for analysis.
*******************************************************************************/
void StreamGenerator::generate_stream(GeneratorObj* generator, ObfuscateObj* obfuscator,
	                        SendObj* sender, double time_start)
{
  timed_interval_start = time_start;
  while (edge_count < num_edges)
  {
    double prob_val = prob_distribution(rand_generator);
    if (prob_val < cp->get_gen_edge_prob())                      //Generate an edge
    {
      // Generate n edges and pass to buf for holding edges generated
      // ONLY GENERATING 1 AT A TIME CURRENTLY (n is parameter in config)
      edge_count++;
      generator->generate_edge(cp->get_mask_width(), cp->get_mask_offset(), edge_datum);
      int32_t msg_length = obfuscator->obfuscate_edge(edge_datum, cp->get_max_msg_len(),
                        msg_buf);
      data_count += sender->send_edge(msg_length, msg_buf);
      rate_throttle(rate, time_start, cp->get_per_packet());
    }
    else
    {
      uint64_t result;
      uint64_t query_id;
      int edge_overflow = 0;
      //YES query
      if (prob_val > (1 - cp->get_gen_yes_prob()))
      {
        int path_length = length_distribution(rand_generator);
        generator->generate_yes(cp->get_mask_width(), cp->get_mask_offset(),path_length,
                        path,query_datum);
        result = 1;
        int j = 0;
        while(j < path_length)
        {
          edge_count++;
          double from_path_prob_val = prob_distribution(rand_generator);
          emit_path_selection(cp->get_mask_width(), cp->get_mask_offset(),&j, from_path_prob_val,
                        cp->get_from_path_prob(), generator, path, edge_datum);
          int32_t msg_length = obfuscator->obfuscate_edge(edge_datum, cp->get_max_msg_len(),
                        msg_buf);
          data_count += sender->send_edge(msg_length, msg_buf);
          rate_throttle(rate, time_start, cp->get_per_packet());
          //May not need since kronecker no longer stores entire graph
          if (edge_count >= num_edges)
          { edge_overflow = 1; break; }
        }
      }
      //NO query
      else
      {
        generator->generate_no(cp->get_mask_width(), cp->get_mask_offset(), query_datum);
        result = 0;
      }
      query_id = query_datum->id;
      //Add query result to buffer and send query
      if (!edge_overflow)
      {
        query_count++;
        results_buf->add_and_check_for_dropped(result, query_id, &dropped_query_count);
        int32_t msg_length = obfuscator->obfuscate_query(query_datum, cp->get_max_msg_len(), msg_buf);
        data_count += sender->send_query(msg_length, msg_buf, 2, query_datum->id);
      }
    }
    /*Probabilities for query types will change with increasing edge_count to allow for low
     *initial probability of generating correctness queries which increases as graph connectivity
     *increases up to some max probability.
    */
    print_timed_interval_output();
  }
  printf("DONE\n\n");
}

/******************************************************************************
 * Parameters:
 * sender - Send Object used to send finish signal to Responder
 * Goal:
 * Terminate stream and process final queries for results reporting
*******************************************************************************/
void StreamGenerator::end_stream(SendObj* sender)
{
  sender->send_done(0);
  pthread_join(listen_thread, NULL);
  results_buf->finish(&dropped_query_count);
  num_packets_sent = sender->get_num_packets_sent();
}

/******************************************************************************
 * Parameters:
 * time_start - The time (in ...) at which the stream began
 * time_stop - The time (in ...) at which the stream terminated
 * Goal:
 * Print all final statistics calculated from processing of graph stream.
*******************************************************************************/
void StreamGenerator::print_results(double time_start, double time_stop)
{
  double elapsed_time = (double)time_stop - time_start;
  printf("~~~~~~FINAL STATS~~~~~~\n");
  printf("NODE_COUNT: %lu\n", (uint64_t)1 << cp->get_log_num_nodes());
  printf("EDGE_COUNT: %ld\n", edge_count);
  printf("QUERY COUNT: %d\n", query_count);
  printf("PACKET COUNT: %lu\n\n", num_packets_sent);
  printf("elapsed time (secs) = %g\n", elapsed_time);
  printf("Data throughput (MB/sec) = %g\n", ((double)data_count/1048576)/elapsed_time);
  printf("generation rate (edges/sec) = %g\n", edge_count/elapsed_time);
  printf("generation rate (queries/sec) = %g\n\n", query_count/elapsed_time);
  printf("CORRECTNESS RESULTS:\n");
  printf("Correct query count: %d\n", correct_query_count);
  printf("Wrong YES query count: %d\n", wrong_yes_query_count);
  printf("Wrong NO query count: %d\n\n", wrong_no_query_count);
  printf("DROP RATES:\n");
  printf("Dropped edge count: %lu\n", edge_count - edges_rcvd_count);
  printf("Dropped query count: %d\n", dropped_query_count);
  printf("Dropped packet count: %lu\n", num_packets_sent - packets_rcvd_count);
  printf("Edge drop rate (%%edges) = %g%%\n", 100*((double)edge_count - edges_rcvd_count)/edge_count);
  printf("Query drop rate (%%queries) = %g%%\n", 100*(double)dropped_query_count/query_count);
  printf("Packet drop rate (%%packets) = %g%%\n\n", 100*((double)num_packets_sent - packets_rcvd_count)/num_packets_sent);
}

/******************************************************************************
 * Parameters:
 * thread_vars - Pointer to thread_struct which contains all variables needed
 * in the listen thread.
 * results_buf - Pointer to ResultsBuffer where all query results are stored
 * to be processed by listen thread.
 * correct_query_count - Pointer to correct query counter to be updated by
 * listen thread.
 * wrong_yes_query_count - Pointer to wrong yes query counter to be updated by
 * listen thread.
 * wrong_no_query_count - Pointer to wrong no query counter to be updated by
 * listen thread.
 * edges_rcvd_count - Pointer to edges received query counter to be updated by
 * listen thread.
 * packets_rcvd_count - Pointer to packets received query counter to be updated by
 * listen thread.
 * Goal:
 * Initialize all values pointed to by thread_vars struct so they can be passed
 * to the listen thread for processing query responses.
*******************************************************************************/
void StreamGenerator::setup_thread_struct(thread_struct* thread_vars, ResultsBuffer* results_buf,
                int* correct_query_count, int* wrong_yes_query_count, int* wrong_no_query_count, 
		uint64_t* edges_rcvd_count, uint64_t* packets_rcvd_count)
{
  thread_vars->results_buf = results_buf;
  thread_vars->correct_query_count = correct_query_count;
  thread_vars->wrong_yes_query_count = wrong_yes_query_count;
  thread_vars->wrong_no_query_count = wrong_no_query_count;
  thread_vars->edges_rcvd_count = edges_rcvd_count;
  thread_vars->packets_rcvd_count = packets_rcvd_count;
  thread_vars->ip_address = cp->get_ip_address();
  thread_vars->to_gen_port = cp->get_to_gen_port();
}

/******************************************************************************
 * Goal:
 * Delay start of TCP client until Responder has initialized TCP server
******************************************************************************/
void StreamGenerator::wait_until_tcp_server_is_ready(uint16_t port_num)
{
  char buf[10000];
  //needs string of port number
  std::string port_as_string = std::to_string(port_num);
  std::string process_string = "netstat -an | grep " + port_as_string;
  std::string s;
  do{
    FILE *proc = popen(process_string.c_str(), "r");
    for (size_t count; (count = fread(buf, 1, sizeof(buf), proc));)
    {
      s += std::string(buf, buf + count);
    }
    pclose(proc);
    usleep(1000);
  } while (s.find(port_as_string.c_str()) == std::string::npos || s.find("LISTEN") == std::string::npos);
}

/******************************************************************************
 * Goal:
 * Obtain time from system clock and return as decimal value in seconds.
*******************************************************************************/
double StreamGenerator::my_clock()
{
  double time;
  struct timeval tv;

  gettimeofday(&tv,NULL);
  time = 1.0 * tv.tv_sec + 1.0e-6 * tv.tv_usec;
  return time;
}

/******************************************************************************
 * Parameters:
 * mask_width - describes the subgraph mask in bits
 * mask_offset - number of bits from the least significant, starts count at 0
 * j - pointer to counter for tracking when to stop emitting edges from path
 * from_path_prob_val - actual random value generated used for choosing whether
 * to select an edge from the path or to generated a new edge
 * from_path_prob - probability of selecting an edge from the path
 * generator - Generator Object used to generate edges not associated with path
 * path - collection of edges indirectly connecting vertices of a guaranteed
 * YES query
 * edge_datum - used to store values of each edge that is emitted
 * Goal:
 * When producing a guaranteed YES query, edges are selected either from
 * a generated path, or randomly generated using the Generator Object to ensure
 * that not all edges of the path associated with the YES query are emitted
 * sequentially.
*******************************************************************************/
void StreamGenerator::emit_path_selection(int mask_width, int mask_offset, int *j, double from_path_prob_val,
				double from_path_prob, GeneratorObj *generator, edge **path, edge *edge_datum)
{
  if (from_path_prob_val < from_path_prob)              //Emit edge from path list
  {
    edge_datum->v0 = path[*j]->v0;
    edge_datum->v1 = path[*j]->v1;
    *j += 1;
  }
  else                                                  //Emit random edge
  {
    generator->generate_edge(mask_width,mask_offset,edge_datum);
  }
}

/******************************************************************************
 * Parameters:
 * rate - The target rate (in edges per sec) for generating the stream
 * time_start - System time of when the stream began (in seconds)
 * per_packet - The number of datums sent in each packet
 * edge_count - The total number of edges generated at this point
 * Goal:
 * Ensure that the rate generation is no greater than some maximum value.
 *
 * Every per_packet number of edges, sleep for the time difference between the
 * actual rate found and the target rate for the number of edges currently
 * generated, if the actual_rate is greater than the target rate
*******************************************************************************/
void StreamGenerator::rate_throttle(int rate, double time_start, int per_packet)
{
  if (rate > 0)
  {
    if (!(edge_count % per_packet)){
      double n = 1.0*edge_count;
      double elapsed = my_clock() - time_start;
      double actual_rate = n/elapsed;
      if (actual_rate > rate) {
        double delay = n/rate - elapsed;
        usleep(1.0e6*delay);
      }
    }
  }
}

/******************************************************************************
 * Goal:
 * Print output at timed intervals
*******************************************************************************/
void StreamGenerator::print_timed_interval_output()
{
  if (cp->get_output_interval() > 0)
  {
    double time_now = my_clock();
    if (time_now >= timed_interval_start + cp->get_output_interval())
    {
      double dynamic_rate = (1.0*(edge_count-prev_edge_count))/(time_now - timed_interval_start);
      double percent_complete = (100.0*edge_count)/num_edges;
      printf("Percent complete: %0.2f%%\t\tDynamic Rate of generation: %g (edges/sec)\n", 
	percent_complete, dynamic_rate);

      timed_interval_start = time_now;
      prev_edge_count = edge_count;
    }
  }
}

/******************************************************************************
 * Parameters:
 * path_length - number of edges in the path
 * path - the actual path
 * Goal:
 * free all malloc'd memory for the path
*******************************************************************************/
void StreamGenerator::free_path(int path_length, edge **path)
{
  for (int i = 0; i < path_length; i++)
  {
    free(path[i]);
  }
  free(path);
}

/******************************************************************************
 * Parameters:
 * thread_vars - the thread_struct pointer
 * Goal:
 * free the malloc'd memory associated with the thread_struct pointer
*******************************************************************************/
void StreamGenerator::free_thread_struct(thread_struct* thread_vars)
{
  free(thread_vars->ip_address);
  free(thread_vars);
}

/******************************************************************************
 * Goal:
 * Destructor for the StreamGenerator class
*******************************************************************************/
StreamGenerator::~StreamGenerator()
{
  free(query_datum);
  free(edge_datum);
  free(msg_buf);
  free_path(cp->get_max_len(), path);
  free_thread_struct(thread_vars);
  delete(results_buf);
}
