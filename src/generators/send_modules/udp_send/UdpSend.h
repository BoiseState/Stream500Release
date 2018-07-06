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

#ifndef UDPSEND_H
#define UDPSEND_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../SendObj.h"
#include "../../../sockets/UDP_Server.h"
#include "../../../../include/udp_throw.h"

#define STOPRATE 100

class UdpSend : public SendObj
{
  protected:
    int num_inside_packet;
    int per_packet;
    UDP_Server* udp_server;
    udp_throw_t *udpt;
    char* ip_address;

  public:
    UdpSend(int init_max_buf_size, int init_per_packet, char *address, uint16_t port);
    uint64_t send_edge(int32_t num_bytes, void *msg_buf);
    uint64_t send_query(int32_t num_bytes, void *msg_buf, int q_priority, uint64_t query_id);
    void send_done(int gen_num);
    uint64_t get_num_packets_sent();
    ~UdpSend();
};

#endif //UDPSEND_H
