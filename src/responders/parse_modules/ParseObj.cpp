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

#include <netinet/in.h>
#include "ParseObj.h"

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
 * Goal:
 * Constructor for the ParseObj class
*******************************************************************************/
ParseObj::ParseObj(){}

/******************************************************************************
 * Parameters:
 * num_bytes - The number of bytes contained in the message to be parsed
 * msg_buf - The buffer containing the message to be parsed
 * vertex0 - A pointer to the first vertex's ID
 * vertex1 - A pointer to the second vertex's ID
 * Goal:
 * Given a message and the length of the message, parse two entities from the
 * message and write a consistent ID for a repeated entity into each of the
 * vertex ID pointers.
 *
 * The default parser does no parsing and only pulls ID's directly from the
 * buffer.
*******************************************************************************/
void ParseObj::parse(int num_bytes, void *msg_buf, uint64_t *vertex0, uint64_t *vertex1)
{
  *vertex0 = be64toh(((uint64_t*)msg_buf)[0]);
  *vertex1 = be64toh(((uint64_t*)msg_buf)[1]);

/*
  //set pointer to ; and find index of ; in msg
  char *secondPart = strchr(msgBuf, ';');
  int semiIndex = secondPart - msgBuf;
  //copy the strings for each vertex id into respective bufs and update lengths
  *v0_len = semiIndex;
  memcpy(v0_buf, msgBuf, *v0_len);
  v0_buf[*v0_len] = '\0';
  *v1_len = numBytes - semiIndex - 1;
  memcpy(v1_buf, &secondPart[1], *v1_len);
  v1_buf[*v1_len] = '\0';
*/
}

/******************************************************************************
 * Goal:
 * Destructor for the ParseObj class
*******************************************************************************/
ParseObj::~ParseObj(){}
