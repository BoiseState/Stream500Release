// Copyright (2013) Sandia Corporation.  Under the terms of Contract
// DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
// certain rights in this software.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.

// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.

// * Neither the name of Sandia Corporation nor the names of contributors
//   to this software may be used to endorse or promote products derived
//   from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Modifications copyright (C) 2018 Boise State University


//=========================================================
// UDP Socket API which comes from Firehose benchmark.
// Some changes have been made from original implementation
// and all changes are commented in code.
//=========================================================

#ifndef _UDP_THROW_H
#define _UDP_THROW_H

// Network headers
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <time.h>
#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _udpt_client_list_t {
     struct sockaddr_in6 sock;
     struct _udpt_client_list_t * next;
} udpt_client_list_t;

typedef struct _udp_throw_t {
     uint16_t port;
     int s;
     struct sockaddr_in6 sock_server;
     socklen_t socklen;
     int clientcnt;
     udpt_client_list_t * clients;
     udpt_client_list_t * next_client;
} udp_throw_t;

static void udp_throw_destroy(udp_throw_t * udpt) {
     if (udpt->s) {
          close(udpt->s);
     }
//****************************************************************************
// Added free of udpt->clients to avoid memory leak in Stream 500 code.
//****************************************************************************
     free(udpt->clients);
     free(udpt);
}

static inline int udp_throw_add_client(udp_throw_t * t, char * address,
                                       uint16_t port) {
     struct addrinfo hints;
     struct addrinfo *result;
     memset(&hints, 0, sizeof(struct addrinfo));

     hints.ai_family = AF_INET6;
     hints.ai_socktype = SOCK_DGRAM;
     hints.ai_flags = AI_V4MAPPED;

     int status;
     if ((status = getaddrinfo(address, NULL, &hints, &result) != 0)) {
          fprintf(stderr, "getaddrinfo %s\n", gai_strerror(status));
          return 0;
     }

     struct addrinfo *res;
     struct sockaddr_in6 * rtry = NULL;

     for (res = result; res; res = res->ai_next) {
          if (res->ai_addr) {
               struct sockaddr_in6 * rsock = (struct sockaddr_in6*)res->ai_addr;
               if (rsock->sin6_family == AF_INET6) {
                    char ipbuf[INET6_ADDRSTRLEN];
                    const char * pipbuf = inet_ntop(AF_INET6,
                                                    (void *)&rsock->sin6_addr,
                                                    ipbuf, INET_ADDRSTRLEN);
                    if (pipbuf) {
                         fprintf(stderr, "trying to connect to %s\n", pipbuf);
                    }
                    rtry = rsock;
                    break;
               }
          }
     }
     if (!rtry) {
          return 0;
     }

//****************************************************************************
// Casting of return from calloc call to udpt_client_list_t* added to avoid
// compilation error in Stream 500 code.
//****************************************************************************
     udpt_client_list_t * cursor = (udpt_client_list_t*)calloc(1, sizeof(udpt_client_list_t));
     if (!cursor) {
          return 0;
     }
     memcpy(&cursor->sock, rtry, sizeof(struct sockaddr_in6));

     cursor->sock.sin6_port = htons(port);
     cursor->next = t->clients;
     t->clients = cursor;
     t->clientcnt++;

     freeaddrinfo(result);
     return 1;
}

static int udp_throw_init_peer(udp_throw_t * udpt, char * peer, uint16_t defaultport) {
     uint16_t port = defaultport;
     int ret = 0;
     char * hostname = NULL;

     char * sep = strchr(peer, '@');
     if (!sep) {
          hostname = strdup(peer);
     }
     else {
          int len = sep - peer;
          hostname = strndup(peer, len);
          port = atoi(sep + 1);
     }

     if (hostname) {
          ret = udp_throw_add_client(udpt, hostname, port);

          free(hostname);
     }

     return ret;
}

static int udp_throw_init_peers_fromfile(udp_throw_t * udpt, char * filename, uint16_t defaultport) {
     FILE * fp;
     int peers = 0;

     fp = fopen(filename, "r");
     if (!fp) {
          return 0;
     }
     char line[1024];
     //read in line

     while (fgets(line, 1024, fp)) {
          int len = strlen(line);
          if (line[len - 1] == '\n') {
               line[len - 1] = '\0';
               len--;
          }
          peers += udp_throw_init_peer(udpt, line, defaultport);
     }

     return peers;
}

//****************************************************************************
// Changed type of buf from char* to void* to match buffer type being used for
// sending data in UDP packets in Stream 500 implementation.
//****************************************************************************
static inline int udp_throw_data(udp_throw_t * thrower, void* buf, int buflen) {
     udpt_client_list_t * client = thrower->next_client;

     //force round-robin throw
     if (!client) {
          //reset to head
          client = thrower->clients;
          if (!client) {
               return 0;
          }
     }
     thrower->next_client = client->next;
     if (sendto(thrower->s, buf, buflen, 0,
                (struct sockaddr *)&(client->sock),
                thrower->socklen) == -1) {
          perror("sendto()");
          return 0;
     }
     return 1;
}


static inline udp_throw_t * udp_throw_init(uint16_t port) {
     udp_throw_t * thrower = (udp_throw_t*)calloc(1, sizeof(udp_throw_t));
     if (!thrower) {
          return NULL;
     }

     if ((thrower->s = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
          perror("socket()");
          return NULL;
     }
     thrower->port = port;

     thrower->socklen = sizeof(struct sockaddr_in6);
     thrower->sock_server.sin6_family = AF_INET6;
     thrower->sock_server.sin6_port = htons(port);
     thrower->sock_server.sin6_addr = in6addr_any;
     if(bind(thrower->s, (struct sockaddr *)(&thrower->sock_server),
             thrower->socklen) == -1) {
          perror("bind()");

          free(thrower);
          return NULL;
     }

     return thrower;
}

#ifdef __cplusplus
}
#endif

#endif

