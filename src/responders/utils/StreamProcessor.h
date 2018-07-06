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

#ifndef STREAM_PROCESSOR_H
#define STREAM_PROCESSOR_H

//#include "../ConfigParser.h"
#include "../resp_eng_init.h"
#include "UnionFindObj.h"
#include "../receive_modules/udp_recv/UdpRecv.h"
#include "../parse_modules/ParseObj.h"
#include "../../sockets/TCP_Server.h"

class StreamProcessor
{
  private:
    // buffers for receiving from and sending on socket
    int max_buf;
    void *meta_data_buf;
    void *buffer;
    void *resp_buf;

    // buffer for holing each message for processing
    void *msg_buf;

    // vertex variables for union-find
    uint64_t vertex0;
    uint64_t vertex1;

    // stats tracking variables
    uint64_t edges_rcvd;
    uint64_t num_rcvd;
    uint64_t num_packets_rcvd;
    uint64_t query_count;

    // setup stop flags
    int num_shut;
    int* shut;

    UnionFindObj* uf;

  public:
//    StreamProcessor(ConfigParser cp);
    StreamProcessor(resp_eng_init* ri);
//    void process_stream(ConfigParser cp, RecvObj* rcvr, ParseObj* parser, int newsockfd);
    void process_stream(resp_eng_init* ri, RecvObj* rcvr, ParseObj* parser, TCP_Server* sender);
    void finish_up(TCP_Server* sender);
    void print_results();
    int shutdown(int *shut, int nsenders, int *nshut, char *buf);
    ~StreamProcessor();
};

#endif //STREAM_PROCESSOR_H
