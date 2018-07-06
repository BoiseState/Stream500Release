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

#include "ObfuscateObj.h"
#include "gtest/gtest.h"
#include <getopt.h>
#include "../../../src/generators/data_types.h"

/*****************************************************************************************
 * Goal:
 * Test class to test the functionality of ObfuscateObj.class
******************************************************************************************/
class ObfuscateObjTest : public testing::Test {
  protected:
	int maxMsgLen;
	edge *edgeDatum;
	query *queryDatum;
	char *msgBuf;
	ObfuscateObj* obf;

	//constructor
	ObfuscateObjTest(){
		maxMsgLen = 55;
		obf = new ObfuscateObj();
		msgBuf = new char[maxMsgLen];	
		edgeDatum = new edge;
		queryDatum = new query;
	}	

	//destructor
	virtual ~ObfuscateObjTest(){
		delete obf;
		delete msgBuf;
		delete edgeDatum;
		delete queryDatum;
	}
};


/*****************************************************************************************
 * Goal:
 * Test that the obfuscated edge is 2 bytes in length
 * test that edge is obfuscated in the order v0, v1
******************************************************************************************/
TEST_F(ObfuscateObjTest,obfuscate_edge_test) { 	
	edgeDatum->v0 = 1;
	edgeDatum->v1 = 2;

	ASSERT_EQ(obf->obfuscate_edge(edgeDatum,maxMsgLen,msgBuf),16); 

	ASSERT_EQ(edgeDatum->v0,be64toh(((int64_t*)msgBuf)[0]));
	ASSERT_EQ(edgeDatum->v1,be64toh(((int64_t*)msgBuf)[1]));
	ASSERT_NE(edgeDatum->v0,be64toh(((int64_t*)msgBuf)[1]));
	ASSERT_NE(edgeDatum->v1,be64toh(((int64_t*)msgBuf)[0]));
}

/*****************************************************************************************
 * Goal:
 * Test that the obfuscated query is 2 bytes in length
 * Test that qurey is obfuscated in the order v0, v1
******************************************************************************************/
TEST_F(ObfuscateObjTest,obfuscate_query_test) { 
	queryDatum->v0 = -5;
	queryDatum->v1 = 6;	

	ASSERT_EQ(obf->obfuscate_query(queryDatum,maxMsgLen,msgBuf),16); 

	ASSERT_EQ(queryDatum->v0,be64toh(((int64_t*)msgBuf)[0]));
	ASSERT_EQ(queryDatum->v1,be64toh(((int64_t*)msgBuf)[1]));
	ASSERT_NE(queryDatum->v0,be64toh(((int64_t*)msgBuf)[1]));
	ASSERT_NE(queryDatum->v1,be64toh(((int64_t*)msgBuf)[0]));
}
