// Copyright (c) 2018 Boise State University 
// All rights reserved.

// Developed by:        Stream500 Team
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

#ifndef STREAM_GENERATOR_H
#define STREAM_GENERATOR_H

#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include "../../ConfigParser.h"
#include "../data_types.h"
#include "../generate_modules/GeneratorObj.h"
#include "../obfuscate_modules/ObfuscateObj.h"
#include "../send_modules/SendObj.h"
#include "ResultsBuffer.h"
#include "ListenThread.h"

class StreamGenerator
{
  private:
    ConfigParser* cp;

    // Variables for results tracking  
    int correct_query_count = 0;
    int wrong_yes_query_count = 0;
    int wrong_no_query_count = 0;
    int dropped_query_count = 0;
    int64_t edge_count = 0;
    int query_count = 0;
    uint64_t data_count = 0;
    uint64_t num_packets_sent = 0;
    uint64_t edges_rcvd_count = 0;
    uint64_t packets_rcvd_count = 0;

    // Variables for listen thread
    int sockfd;

    ResultsBuffer *results_buf;
    thread_struct* thread_vars;
    pthread_t listen_thread;

    // Buffer for storing obfuscated edges
    void *msg_buf;

    // Datums and datum array for storing queries, edges, and paths
    query *query_datum;
    edge *edge_datum;
    edge **path;
    int64_t num_edges;
    int rate;
    uint64_t prev_edge_count;
    double timed_interval_start;

    // Random number generators
    std::default_random_engine rand_generator;
    std::random_device device;
    std::uniform_real_distribution<double> prob_distribution;    
    std::uniform_int_distribution<int> length_distribution;

  public:
    StreamGenerator(ConfigParser* init_cp);
    void generate_stream(GeneratorObj* generator, ObfuscateObj* obfuscator, SendObj* sender,
		double time_start);
    void end_stream(SendObj* sender);
    void print_results(double time_start, double time_stop);
//    void setup_thread_struct(thread_struct* thread_vars, ResultsBuffer* results_buf, int sockfd,
//                int* correct_query_count, int* wrong_yes_query_count, int* wrong_no_query_count,
//		uint64_t* edges_rcvd_count, uint64_t* packets_rcvd_count);
    void setup_thread_struct(thread_struct* thread_vars, ResultsBuffer* results_buf, 
		int* correct_query_count, int* wrong_yes_query_count, int* wrong_no_query_count,
		uint64_t* edges_rcvd_count, uint64_t* packets_rcvd_count);
    void emit_path_selection(int mask_width, int mask_offset, int *j, double from_path_prob_val, 
		double from_path_prob, GeneratorObj *generator, edge **path, edge *edge_datum);
    void free_path(int path_length, edge **path);
    void free_thread_struct(thread_struct* thread_vars);
    double my_clock();
    void wait_until_tcp_server_is_ready(uint16_t port_num);
    void rate_throttle(int rate, double time_start, int per_packet);
    void print_timed_interval_output();
    ~StreamGenerator();
};

#endif //STREAM_GENERATOR_H
