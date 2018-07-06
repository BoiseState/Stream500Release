// Copyright (c) 2018 Boise State University 
// All rights reserved.

// Developed by: 	    Stream500 Team
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

#include "TwitterParser.h"
#include "../ParseObj.cpp"
#include "gtest/gtest.h"
#include <getopt.h>
#include <string.h>
#include "TwitterParser.h"
#include "../../../generators/data_types.h"


/*****************************************************************************************
 * Goal:
 * To unit test the functionality of TwitterParser.cpp class
******************************************************************************************/
class TwitterParserTest : public testing::Test {
  protected:
	uint64_t *vertex0;
    uint64_t *vertex1;
    edge *edge_datum;
    char* msg_buf;
	TwitterParser *twt_parser;	
	int max_buf_size;
	
	//constructor
    TwitterParserTest(){
      max_buf_size = 255;
      vertex0 = new uint64_t;
      vertex1 = new uint64_t;
      edge_datum = new edge;
      edge_datum->v0 = 35;
      edge_datum->v1 = 40;
      msg_buf = new char[max_buf_size];
      twt_parser = new TwitterParser();
    }

    //destructor
    virtual ~TwitterParserTest(){
      delete vertex0;
      delete vertex1;
      delete edge_datum;
      delete msg_buf;
      delete twt_parser;
    }
};


/*********************************************************************************************
 * Goal:
 * To test the parse method of TwitterParser.cpp class
 * The obfuscated message is generated using the regular expression. The vertices obfuscated
 * are 35 and 40. The obfuscated message is store in obfuscated_edge_or_query variable.
 * 
**********************************************************************************************/
TEST_F(TwitterParserTest, twitter_style_parser_test_1) { 
  //obfuscated message of edge_datum->v0 and edge_datum->v1
  char* obfuscated_edge_or_query = "b352dddbdcd f35lej a1cddbddd q40qrx";
  int32_t num_bytes = snprintf(msg_buf, max_buf_size, obfuscated_edge_or_query);

  //call the parse method to parse out the vertices in the given edge or query
  twt_parser->parse(num_bytes, msg_buf, vertex0, vertex1);
  ASSERT_EQ(*vertex0,edge_datum->v0);
  ASSERT_EQ(*vertex1,edge_datum->v1);
}

/*********************************************************************************************
 * Goal:
 * To test the parse method of TwitterParser.cpp class
 * The obfuscated message is generated using the regular expression. The vertices obfuscated
 * are 35 and 40. The obfuscated message is store in obfuscated_edge_or_query variable.
 * 
**********************************************************************************************/
TEST_F(TwitterParserTest, twitter_style_parser_test_2) { 
  //obfuscated message of edge_datum->v0 and edge_datum->v1
  char* obfuscated_edge_or_query = "a147cddaddd h35kmn b1000cccaccd s40oyq";
  int32_t num_bytes = snprintf(msg_buf, max_buf_size, obfuscated_edge_or_query);

  //call the parse method to parse out the vertices in the given edge or query
  twt_parser->parse(num_bytes, msg_buf, vertex0, vertex1);
  ASSERT_EQ(*vertex0,edge_datum->v0);
  ASSERT_EQ(*vertex1,edge_datum->v1);
}