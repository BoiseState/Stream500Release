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

#include "UdpRecv.h"

UdpRecv::UdpRecv(uint16_t port)
{
  socket = ::socket(PF_INET6, SOCK_DGRAM, 0);
  if (socket == -1)
    throw std::runtime_error(std::string("socket(): ") + ::strerror(errno));

  struct sockaddr_in6 address;
  ::memset(&address, 0, sizeof(address));
  address.sin6_family = AF_INET6;
  address.sin6_addr = in6addr_any;
  address.sin6_port = htons(port);
  if (-1 == ::bind(socket, reinterpret_cast<sockaddr*>(&address),
                   sizeof(address)))
    throw std::runtime_error(std::string("bind(): ") + ::strerror(errno));
}
    
int UdpRecv::receive(void *recv_buf, int max_to_receive)
{
  return ::recv(socket,recv_buf,max_to_receive,0);
}

UdpRecv::~UdpRecv()
{
  close(socket);
}
