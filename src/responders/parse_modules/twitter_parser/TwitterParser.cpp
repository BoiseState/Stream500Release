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

#include"TwitterParser.h"
#include <string.h>
#include <netinet/in.h>
#include<iostream>
#include <regex>
#pragma GCC diagnostic ignored "-Wpointer-arith"

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
 * Constructor for the TwitterParser class
*******************************************************************************/
TwitterParser::TwitterParser(){}

/******************************************************************************
 * Parameters:
 * num_bytes - The number of bytes contained in the message to be parsed
 * msg_buf - The buffer containing the message to be parsed
 * vertex0 - A pointer to the first vertex's ID
 * vertex1 - A pointer to the second vertex's ID
 * Goal:
 * Given an obfuscated messsage, parse out the vertices ids using regular expression
 * Use the same regular expression that was used to obfuscate edge/query
*******************************************************************************/
void TwitterParser::parse(int num_bytes, void *msg_buf, uint64_t *vertex0, uint64_t *vertex1)
{  
  ((char*)msg_buf)[num_bytes]='\0'; 
  const std::string s = (char*)msg_buf;

  //Parse out vertex0
  std::regex rgx_for_vertex0("[e-j](.*?)[e-n]{3}");
  std::smatch match_for_vertex0;
  if (std::regex_search(s.begin(), s.end(), match_for_vertex0, rgx_for_vertex0)) *vertex0 = stoi(match_for_vertex0[1]);

  //Parse out vertex1
  std::regex rgx_for_vertex1("[o-t](.*?)[o-z]{3}");
  std::smatch match_for_vertex1;
  if (std::regex_search(s.begin(), s.end(), match_for_vertex1, rgx_for_vertex1)) *vertex1 = stoi(match_for_vertex1[1]);
}

/******************************************************************************
 * Goal:
 * Destructor for the TwitterParser class
*******************************************************************************/
TwitterParser::~TwitterParser(){}
