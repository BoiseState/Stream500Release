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

#include "ListenThread.h"

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
 * ptr - void* ptr to struct containing all variables needed in method
 * Goal:
 * Method for pthread(s) that receives and processes query results from
 * Responder.
*******************************************************************************/
void *listen_to_socket(void *ptr)
{
  const int result_size = 16;    //Two 64-bit integers
  char listen_buf[result_size];
  thread_struct* thread_vars = (thread_struct *)ptr;
  ResultsBuffer* results_buf = thread_vars->results_buf;
  char *confirm_buf = (char*)malloc(4);
  memset(confirm_buf,0,4);

  //Setup TCP socket for receiving responses
  TCP_Client* tcp_client = new TCP_Client(thread_vars->ip_address, thread_vars->to_gen_port);

  //Listen on socket for result response.
  printf("Starting listen thread\n\n");

  while (true)
  {
    int num_bytes = tcp_client->tcp_cli_recv(&listen_buf[0], result_size, 0);
    if(num_bytes < result_size) //Received a DONE packet
    { 
      tcp_client->tcp_cli_send(confirm_buf, 4, 0);
      break; 
    }
    //Unpack result
    uint64_t query_result_id = be64toh(((uint64_t*)listen_buf)[0]);
    uint64_t result_val = be64toh(((uint64_t*)listen_buf)[1]);
    memset(listen_buf,0,result_size);
    //Search for result in results buffer and update results stats accordingly.
    results_buf->search_and_update_stats(result_val, query_result_id, thread_vars->correct_query_count,
                        thread_vars->wrong_yes_query_count, thread_vars->wrong_no_query_count);
  }

  tcp_client->tcp_cli_recv(&listen_buf[0], result_size, 0);
  *thread_vars->edges_rcvd_count = be64toh(((uint64_t*)listen_buf)[0]);
  *thread_vars->packets_rcvd_count = be64toh(((uint64_t*)listen_buf)[1]);
  tcp_client->tcp_cli_send(confirm_buf, 4, 0);
  free(confirm_buf);
  delete(tcp_client);
  pthread_exit(0);
}

