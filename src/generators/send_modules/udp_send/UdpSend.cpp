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

#include "UdpSend.h"

UdpSend::UdpSend(int max_datum_size, int init_per_packet, char* address, uint16_t port)
{
  num_packets_sent = 0;
  num_inside_packet = 0;
  curr_offset = 4;
  per_packet = init_per_packet;
  max_buf_size = max_datum_size * per_packet;
  out_buf = malloc(max_buf_size);
  ip_address = address;

//  udp_serv = new UDP_Server(address, port);
  
  udpt = udp_throw_init(0);
  if (!udpt) {
    fprintf(stderr,"ERROR: unable to generate data\n");
    exit(1);
  }
  udp_throw_init_peer(udpt, address, port);	//May want to consider multiple addresses...
  if (!udpt->clientcnt) {
    fprintf(stderr,"ERROR: no clients\n");
    free(out_buf);
    exit(1);
  }  
}

uint64_t UdpSend::send_edge(int32_t num_bytes, void *msg_buf)
{
  int32_t endian_d_type = htonl(0);
  int32_t endian_n_bytes = htonl(num_bytes);
  memcpy(((char*)out_buf)+curr_offset, &endian_d_type, 4);
  curr_offset += 4;
  memcpy(((char*)out_buf)+curr_offset, &endian_n_bytes, 4);
  curr_offset += 4;
  memcpy(((char*)out_buf)+curr_offset, msg_buf, num_bytes);
  curr_offset += num_bytes;
  num_inside_packet++;
  if (num_inside_packet >= per_packet)
  {
    int32_t endian_num_in_pkt = htonl(num_inside_packet);
    memcpy(out_buf, &endian_num_in_pkt, 4);
    udp_throw_data(udpt, out_buf, curr_offset);
//    udp_server->udp_serv_send(out_buf, curr_offset, 0);
    num_packets_sent++;
    uint64_t num_bytes_sent = (uint64_t)curr_offset;
    curr_offset = 4;
    num_inside_packet = 0;
    return num_bytes_sent;
  }
  return 0;
}

uint64_t UdpSend::send_query(int32_t num_bytes, void *msg_buf, int q_priority, uint64_t query_id)
{
  int32_t endian_d_type = htonl(q_priority);
  int32_t endian_n_bytes = htonl(num_bytes);
  uint64_t endian_id = htobe64(query_id);
  memcpy(((char*)out_buf)+curr_offset, &endian_d_type, 4);
  curr_offset += 4;
  memcpy(((char*)out_buf)+curr_offset, &endian_n_bytes, 4);
  curr_offset += 4;
  memcpy(((char*)out_buf)+curr_offset, msg_buf, num_bytes);
  curr_offset += num_bytes;
  memcpy(((char*)out_buf)+curr_offset, &endian_id, 8);
  curr_offset += 8;
  num_inside_packet++;
  if (num_inside_packet >= per_packet)
  {
    int32_t endian_num_in_pkt = htonl(num_inside_packet);
    memcpy(out_buf, &endian_num_in_pkt, 4);
    udp_throw_data(udpt, out_buf, curr_offset);
//    udp_server->udp_serv_send(out_buf, curr_offset, 0);
    num_packets_sent++;
    uint64_t num_bytes_sent = (uint64_t)curr_offset;
    curr_offset = 4;
    num_inside_packet = 0;
    return num_bytes_sent;
  }
  return 0;
}

void UdpSend::send_done(int gen_num)
{
  int32_t endian_num_in_pkt = htonl(num_inside_packet);
  memcpy(out_buf, &endian_num_in_pkt, 4);
  udp_throw_data(udpt, out_buf, curr_offset);
//  udp_server->udp_serv_send(out_buf, curr_offset, 0);
  num_packets_sent++;

  int32_t endian_gen_num = htonl(gen_num);
  memcpy(out_buf, &endian_gen_num, 4);
  for(int i = 0; i < 100; i++)
  {
    udp_throw_data(udpt, out_buf, 4);
//    udp_server->udp_serv_send(out_buf, 4, 0);
    usleep(1000000/STOPRATE);
  }
}

uint64_t UdpSend::get_num_packets_sent()
{
  return num_packets_sent;
}

UdpSend::~UdpSend()
{
  udp_throw_destroy(udpt);
//  delete(udp_server);
  free(ip_address);
  free(out_buf);
}
