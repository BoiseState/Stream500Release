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

#include "TCP_Client.h"

TCP_Client::TCP_Client(char* address, uint16_t init_port)
{
  struct sockaddr_in serv_addr;
  struct hostent *server;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    printf("ERROR opening socket\n");
  server = gethostbyname(address);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
       (char *)&serv_addr.sin_addr.s_addr,
       server->h_length);
  serv_addr.sin_port = htons(init_port);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    printf("ERROR connecting");
}

void TCP_Client::tcp_cli_send(void* send_buf, int num_bytes, int flags)
{
  send(sockfd, send_buf, num_bytes, flags);
}

int TCP_Client::tcp_cli_recv(void* recv_buf, int num_bytes, int flags)
{
  return recv(sockfd, recv_buf, num_bytes, flags);
}

TCP_Client::~TCP_Client()
{
  close(sockfd);
}
