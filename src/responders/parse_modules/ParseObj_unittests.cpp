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

#include<inttypes.h>
#include<stdlib.h>
#include "gtest/gtest.h"
#include "ParseObj.h"
#include "../../generators/data_types.h"


/*****************************************************************************************
 * Goal:
 * To unit test the functionality of ParseObj.cpp class
******************************************************************************************/
class ParseObjTest : public testing::Test {
  protected:  
    //Variables
    int max_msg_len;
    char *msg_buf;
    uint64_t *vertex0;
    uint64_t *vertex1;
    uint64_t endian_v0;
    uint64_t endian_v1;  
    ParseObj *prse;
    edge *edge_datum;

    //constructor
    ParseObjTest(){
      max_msg_len = 55;
      vertex0 = new uint64_t;
      vertex1 = new uint64_t;
      edge_datum = new edge;
      msg_buf = new char[max_msg_len];
      prse = new ParseObj();
    }

    //destructor
    virtual ~ParseObjTest(){
      delete vertex0;
      delete vertex1;
      delete edge_datum;
      delete msg_buf;
      delete prse;
    }
};


/*****************************************************************************************
 * Goal:
 * Verify that the obfuscated message are parsed in the same order as they were obfuscated
******************************************************************************************/
TEST_F(ParseObjTest, parse_test) { 
  //two vertices of a query
  edge_datum->v0 = 100;
  edge_datum->v1 = 9345;
  int num_bytes_in_obfuscated_msg = 16; //should be 16 because each vertex is 8 bytes
  
  //convert to endian
  endian_v0 = htobe64(edge_datum->v0);
  endian_v1 = htobe64(edge_datum->v1);

  memcpy(msg_buf, &endian_v0, 8);
  memcpy(msg_buf+8, &endian_v1, 8);

  prse->parse(num_bytes_in_obfuscated_msg, msg_buf,vertex0,vertex1);
  
  ASSERT_EQ(*vertex0,edge_datum->v0);
  ASSERT_EQ(*vertex1,edge_datum->v1);
  ASSERT_NE(*vertex0,edge_datum->v1);
  ASSERT_NE(*vertex1,edge_datum->v0);
}