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

#include "TCP_Server.h"

void handler(int signum);
void preclean();
void cleanup();
int sockfd;
int newsockfd;
struct sigaction socket_cleanup;
struct sigaction old_action;

TCP_Server::TCP_Server(uint16_t port)
{
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
     perror("ERROR opening socket");
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
    perror("ERROR on binding");
  }
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd < 0)
  {
    perror("ERROR on accept");
  }
  socket_cleanup.sa_handler = handler;
  sigemptyset (&socket_cleanup.sa_mask);
  socket_cleanup.sa_flags = 0;
  sigaction(SIGINT, &socket_cleanup, &old_action);
}

void TCP_Server::tcp_serv_send(void* send_buf, int num_bytes, int flags)
{
  send(newsockfd, send_buf, num_bytes, flags);
}

void TCP_Server::tcp_serv_recv(void* recv_buf, int num_bytes, int flags)
{
  recv(newsockfd, recv_buf, num_bytes, flags);
}

void handler(int signum)
{
  preclean();
  cleanup();
  sigaction(SIGINT, &old_action, NULL);
  raise(SIGINT);
}

void preclean()
{
  void* confirm_buf = malloc(4);

  void* buf = malloc(4);
  memset(buf, 0, 4);
  send(newsockfd, buf, 4, 0);
  recv(sockfd, confirm_buf, 4, 0);

  void *last_buf = malloc(16);
  memset(last_buf, 0, 16);
  send(newsockfd, last_buf, 16, 0);
  recv(sockfd, confirm_buf, 4, 0);

  free(buf);
  free(last_buf);
  free(confirm_buf);
}

void cleanup()
{
  close(sockfd);
}

TCP_Server::~TCP_Server()
{
  cleanup();
}


