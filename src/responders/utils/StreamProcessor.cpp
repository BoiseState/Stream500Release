// Copyright (c) 2018 Boise State University 
// All rights reserved.

// Developed by:       Stream500 Team
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

#include "StreamProcessor.h"

#if defined(__linux__)
#  include <endian.h>
#elif defined(__FreeBSD__) || defined(__NetBSD__)
#  include <sys/endian.h>
#elif defined(__OpenBSD__)
#  include <sys/types.h>
#  define be16toh(x) betoh16(x)
#  define be32toh(x) betoh32(x)
#  define be64toh(x) betoh64(x)
#endif

/******************************************************************************
 * Parameters:
 * ri - The collection of configuration parameters to be used by the Responder
 * Goal:
 * Constructor for the StreamProcessor class. Sets up all of the variables
 * needed for analyzing the graph stream
*******************************************************************************/
//StreamProcessor::StreamProcessor(ConfigParser cp)
StreamProcessor::StreamProcessor(resp_eng_init* ri)
{
  max_buf = ri->get_max_msg_len()*ri->get_per_packet();
  meta_data_buf = malloc(2*sizeof(int32_t));
  buffer = malloc(max_buf);
  resp_buf = malloc(ri->get_max_msg_len());
  msg_buf = malloc(ri->get_max_msg_len());

  edges_rcvd = 0;
  num_rcvd = 0;
  num_packets_rcvd = 0;
  query_count = 0;

  num_shut = 0;
  shut = (int*)malloc(ri->get_num_gen()*sizeof(int));
  for (int i = 0; i < ri->get_num_gen(); i++)
  {
    shut[i] = 0;
  }

  uf = new UnionFindObj((uint64_t)1 << ri->get_log_num_nodes());
}

/******************************************************************************
 * Parameters:
 * ri - The collection of configuration parameters to be used by the Responder
 * rcvr - the RecvObj to be used for receiving from the Generator
 * parser - The ParseObj to be used for parsing entities from messages received
 * newsockfd - The socket to be used for sending responses back to the Generator
 * Goal:
 * Main loop for processing all of the edges and queries received from the
 * Generator. Performs Union-Find operations to track connectedness of edges
 * received.
*******************************************************************************/
//void StreamProcessor::process_stream(ConfigParser cp, RecvObj* rcvr, ParseObj* parser, int newsockfd)
void StreamProcessor::process_stream(resp_eng_init* ri, RecvObj* rcvr, ParseObj* parser, TCP_Server* sender)
{

  // loop on reading packets
  while (true) {

    // read a packet with n bytes
    int num_bytes_rcvd = rcvr->receive(buffer, max_buf);

    // exit if have received STOP packet from every sender
    if (num_bytes_rcvd < 8) {
      if (shutdown(&shut[0], ri->get_num_gen(), &num_shut, &((char*)buffer)[0])) break;
      continue;
    }

    int32_t num_msgs = ntohl(((int32_t*)buffer)[0]);
    int curr_offset = 4;

    // process datums in packet
    num_packets_rcvd++;
    for (int i = 0; i < num_msgs; i++)
    {
      num_rcvd++;
      memcpy(meta_data_buf, ((char*)buffer)+curr_offset, 8);
      int32_t datum_type = ntohl(((int32_t*)meta_data_buf)[0]);
      curr_offset += 4;
      int32_t num_bytes = ntohl(((int32_t*)meta_data_buf)[1]);
      curr_offset += 4;
      memcpy(msg_buf, ((char*)buffer)+curr_offset, num_bytes);
      curr_offset += num_bytes;

      parser->parse(num_bytes, msg_buf, &vertex0, &vertex1);
      //PROCESS THE TWO USERIDS WITH UNION-FIND
      uint64_t root0 = uf->find(vertex0);
      uint64_t root1 = uf->find(vertex1);
      if (datum_type == 0)      // Edge
      {
        edges_rcvd++;
        // Perform union-find on incoming edges
//        if (uf->find(vertex0, vertex1))
        if (root0 == root1)
        {
          continue;
        }
        else
        {
//          uf->unite(vertex0, vertex1);
          uf->unite(root0, root1);
        }
      }
      else if (datum_type == 1) // Standard Correctness Query
      {
//        if (uf->find(vertex0, vertex1))
        if (root0 == root1)
        {
        }
        else
        {
        }
      }
      else if (datum_type == 2) // High Priority Correctness Query
      {
        //need to create temp buffer for query ID to properly align index
        //into buffer when casting value at buffer+curr_offset to uint64_t
        void *id_buf = malloc(sizeof(uint64_t));
        memcpy(id_buf, ((char*)buffer)+curr_offset, 8);
        uint64_t query_id = be64toh(((uint64_t*)id_buf)[0]);
        curr_offset += 8;
        free(id_buf);
        uint64_t endian_id = htobe64(query_id);
        memcpy(resp_buf, &endian_id, 8);
        uint64_t endian_resp;
//        if (uf->find(vertex0, vertex1))
        if (root0 == root1)
        {
          endian_resp = htobe64(1);
        }
        else
        {
          endian_resp = htobe64(0);
        }
        memcpy(((char*)resp_buf)+8, &endian_resp, 8);
        sender->tcp_serv_send(resp_buf, 16, 0);
        memset(resp_buf, 0, 16);
        query_count++;
      }
    }
    memset(buffer, 0, max_buf);
  }
}

/******************************************************************************
 * Parameters:
 * newsockfd - Socket to use for sending values
 * Goal:
 * Send final stats (number of edges and packets received) to Generator when
 * run is complete.
*******************************************************************************/
void StreamProcessor::finish_up(TCP_Server* sender)
{
  // send Done packet
  void* confirm_buf = malloc(4);
  uint64_t endian_done = htobe64(0);
  memcpy(resp_buf, &endian_done, 8);
  sender->tcp_serv_send(resp_buf, 8, 0);
  sender->tcp_serv_recv(confirm_buf, 4, 0);
  // send Received stats
  uint64_t endian_edges_rcvd = htobe64(edges_rcvd);
  uint64_t endian_num_packets_rcvd = htobe64(num_packets_rcvd);
  memcpy(resp_buf, &endian_edges_rcvd, 8);
  memcpy(((char*)resp_buf)+8, &endian_num_packets_rcvd, 8);
  sender->tcp_serv_send(resp_buf, 16, 0);
  sender->tcp_serv_recv(confirm_buf, 4, 0);
  free(confirm_buf);
}

/******************************************************************************
 * Goal:
 * Print the final stats calculated by the Responder on the run.
*******************************************************************************/
void StreamProcessor::print_results()
{
  printf("Final results:\n");
  printf("Total datums received:\t%lu\n", num_rcvd);
  printf("Total packets received:\t%lu\n", num_packets_rcvd);
  printf("Total query count:\t%lu\n", query_count);
}

/******************************************************************************
 * Parameters:
 * shut - Array of integers representing shutdown status of each generator
 * nsenders - The number of generators sending packets
 * nshut - The running count of number of generators in shutdown status
 * buf - The bytes received in a DONE packet with generator number
 * Goal:
 * Determine if the entire generation process has completed.
 *
 * If the number of generators in shutdown status matches the number of generators
 * then generation has completed and the Responder should shutdown processing
 * Return:
 * 1 if the number of generators that are DONE matches the total number of
 * generators, else 0
*******************************************************************************/
int StreamProcessor::shutdown(int *shut, int nsenders, int *nshut, char *buf)
{
  int32_t iwhich = ntohl(((int32_t*)buf)[0]);

  if (shut[iwhich]) return 0;
  shut[iwhich] = 1;
  (*nshut)++;
  if (*nshut == nsenders) return 1;
  return 0;
}

/******************************************************************************
 * Goal:
 * Destructor for the StreamProcessor class
*******************************************************************************/
StreamProcessor::~StreamProcessor()
{
  free(meta_data_buf);
  free(buffer);
  free(msg_buf);
  free(resp_buf);
  free(shut);
  delete(uf);
}
