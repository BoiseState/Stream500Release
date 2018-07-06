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

#include "UDP_Server.h"


int serv_sockfd, cli_sockfd;


UDP_Server::UDP_Server(char* address, uint16_t init_port)
{
  struct sockaddr_in6 serv_addr;

  if ((serv_sockfd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) < 0) 
  {
    perror("socket()");
  }
  
  serv_addr.sin6_family = AF_INET6;
  serv_addr.sin6_port = htons(init_port);
  serv_addr.sin6_addr = in6addr_any;
  if (bind(serv_sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
  {
    perror("bind()");
  }


  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int s;
  struct sockaddr_storage peer_addr;
  socklen_t peer_addr_len;
  ssize_t nread;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET6;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
  hints.ai_flags = AI_V4MAPPED;    /* For wildcard IP address */
//  hints.ai_protocol = 0;          /* Any protocol */
//  hints.ai_canonname = NULL;
//  hints.ai_addr = NULL;
//  hints.ai_next = NULL;

  s = getaddrinfo(address, NULL, &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

 /* getaddrinfo() returns a list of address structures.
    Try each address until we successfully bind(2).
    If socket(2) (or bind(2)) fails, we (close the socket
    and) try the next address. */

  for (rp = result; rp != NULL; rp = rp->ai_next) 
  {
    cli_sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (cli_sockfd == -1)
      continue;

    if (bind(cli_sockfd, rp->ai_addr, rp->ai_addrlen) == 0)
      break;                  /* Success */

    close(cli_sockfd);
  }

  if (rp == NULL) {               /* No address succeeded */
    fprintf(stderr, "Could not bind\n");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(result);
}

void UDP_Server::udp_serv_send(void* send_buf, int num_bytes, int flags)
{
  send(serv_sockfd, send_buf, num_bytes, flags);
}

void UDP_Server::udp_serv_recv(void* recv_buf, int num_bytes, int flags)
{
  recv(serv_sockfd, recv_buf, num_bytes, flags);
}

UDP_Server::~UDP_Server()
{
  close(serv_sockfd);
}

