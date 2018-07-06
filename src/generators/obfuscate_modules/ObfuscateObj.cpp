// Copyright (c) 2018 Boise State University 
// All rights reserved.

// Developed by:      Stream500 Team
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

#include "ObfuscateObj.h"
#include <string.h>
#include <netinet/in.h>

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
 * Constructor for ObfuscateObj
*******************************************************************************/
ObfuscateObj::ObfuscateObj(){}

/******************************************************************************
 * Parameters:
 * e - edge pointer containing values of an edge
 * max_buf_size - maximum size of message for an obfuscated edge
 * msg_buf - buffer for storing the obfuscated edge
 * Goal:
 * Obfuscate an edge so the Responder has to do some processing to parse
 * entities associated with a given edge (default obfuscator does no
 * obfuscation and allows processing on vertex ID's directly)
*******************************************************************************/
int32_t ObfuscateObj::obfuscate_edge(edge *e, int max_buf_size, void *msg_buf)
{
  uint64_t endian_v0 = htobe64(e->v0);
  uint64_t endian_v1 = htobe64(e->v1);
  memcpy(msg_buf, &endian_v0, 8);
  memcpy(((char*)msg_buf)+8, &endian_v1, 8);
  return 2*sizeof(int64_t);
}

/******************************************************************************
 * Parameters:
 * q - query pointer containing values of a query
 * max_buf_size - maximum size of message for an obfuscated query
 * msg_buf - buffer for storing the obfuscated query
 * Goal:
 * Obfuscate the vertex ID's of a query the same way as edges are obfuscated
 * so they appear the same to the Responder. Query ID's should not be obfuscated
 * so they can be matched when checking results. 
*******************************************************************************/
int32_t ObfuscateObj::obfuscate_query(query *q, int max_buf_size, void *msg_buf)
{
  uint64_t endian_v0 = htobe64(q->v0);
  uint64_t endian_v1 = htobe64(q->v1);
  memcpy(msg_buf, &endian_v0, 8);
  memcpy(((char*)msg_buf)+8, &endian_v1, 8);
  return 2*sizeof(int64_t);
}

/******************************************************************************
 * Goal:
 * Destructor for ObfuscateObj
*******************************************************************************/
ObfuscateObj::~ObfuscateObj(){}
