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
#include<iostream>
#include "GeneratorObj.h"
#include "erdosrenyi/ErdRenGen.cpp"
#include "gtest/gtest.h"


/******************************************************************************
 * Parameters:
 * width - describes the subgraph mask in bits
 * offset - number of bits from the least significant, starts count at 0
 * v0 - the first vertex
 * v1 - second vertex
 * has_been_switched - a flag to ensure a given pair of vertices is only
 * processed, at max, two times.
 * Goal:
 * Make the the subgraph masks match so that it is allowable that they be
 * connected. 
 * 
 * If they aren't already in the same subgraph - change v1's mask to match
 * v0. 
 *
*******************************************************************************/

class GeneratorObjTest : public testing::Test {
  public:   
    int width;
    int offset;
    int64_t mask; //Mask value calculated using the width and offset

    int64_t init_num_nodes;
    int64_t init_num_edges;
    int erd_ren_rand_seed;

    uint64_t i;
    uint64_t j;
    uint64_t *v0;
    uint64_t *v1;
    bool connected;
    int flag ;

    edge *e;
    query *q;
    query *queryDatum;
    edge *edgeDatum;
    GeneratorObj *generateobj;

  //constructor
    GeneratorObjTest(){
      width = 3;
      offset = 1;
      mask = 14; //Mask value calculated using the width and offset

      init_num_nodes = 1000;
      init_num_edges = 10;
      erd_ren_rand_seed = 12;

      e = new edge;
      q = new query;
      queryDatum = new query;
      edgeDatum = new edge;
      generateobj = new ErdRenGen(init_num_nodes,init_num_edges,erd_ren_rand_seed);
    }
  //destructor
    virtual ~GeneratorObjTest(){
      delete e;
      delete q;
      delete queryDatum;
      delete edgeDatum;
      delete generateobj;
    }
};

// /* ensure_same_subgraph function should generate vertices that
//  fall in the same subgraph. This is tested using the mask bits */
TEST_F(GeneratorObjTest, ensure_same_subgraph_Test1) {
//Define variables
  i = 9;
  j = 20;
  v0 = &i;
  v1 = &j;
  connected = false;		
  generateobj->ensure_same_subgraph(width, offset, v0, v1,0);

  if((*v0 & mask) == (*v1 & mask)){
   connected = true;
 }	
 ASSERT_EQ(connected,true);
}

/* Even If two identical vertices are sent, they should belong to the 
  same subgraph and their value should not change after calling 
  ensure_same_subgraph function. */
TEST_F(GeneratorObjTest, ensure_same_subgraph_Test2) {
//Define variables
  i =20;
  j=20;
  v0 = &i;
  v1 = &j;     
  generateobj->ensure_same_subgraph(width, offset, v0, v1,0); 
  ASSERT_EQ(*v0,*v1);
}

// Test the ensureDisConnected 
TEST_F(GeneratorObjTest, ensure_different_subgraph_Test_1) {
//Define variables
  i =9;
  j=20;
  v0 = &i;
  v1 = &j;
  connected = true;  

  generateobj->ensure_different_subgraph(width, offset, v0, v1,0);
  if((*v0 & mask) != (*v1 & mask)){
   connected = false;
 }	 	
 ASSERT_EQ(connected,false);
}

/* If two identical vertices are sent, they belong to the same 
 subgraph and their value should not change after calling 
 ensure_different_subgraph function */
TEST_F(GeneratorObjTest, ensure_different_subgraph_Test_2) {
//Define variables
  i = 20;
  j = 20;
  v0 = &i;
  v1 = &j;     
  generateobj->ensure_different_subgraph(width, offset, v0, v1,0); 
  ASSERT_NE(*v0,*v1);
}

/* Test that the generated edge is formed of connected vertices.
 The connected*/
TEST_F(GeneratorObjTest, generate_edge_is_connected_Test) {	
  generateobj->generate_edge(width, offset, e);
  connected = false;
// Check that the generated edge is formed of connected vertices
  if((e->v0 & mask) == (e->v1 & mask)){
   connected = true;
 }	
 ASSERT_EQ(connected,true);
}

// Test that the GenerateNo queries generate non-connected vertices.
TEST_F(GeneratorObjTest, generate_no_Test) {	
	generateobj->generate_no(width,offset,q);
  connected = true;
	// Check that the generated edge is formed of connected vertices
  if((q->v0 & mask) != (q->v1 & mask)){
 		connected = false;
 	}	
 	ASSERT_EQ(connected,false);
}

// Test that the generate_yes generate connected vertices.
TEST_F(GeneratorObjTest, generate_yes_Test) {
	// Create datums and datum array for storing queries, edges, and paths
  int max_len = 6;
  edge **path;
  path = (edge **)malloc(max_len * sizeof(edge *));
  for (int i = 0; i < max_len; i++)
  {
    path[i] = (edge *)malloc(sizeof(edge));
  }

  generateobj-> generate_yes(width, offset, max_len, path, queryDatum);
  connected = false;
	// Check that the generated edge is formed of connected vertices
  if((queryDatum->v0 & mask) == (queryDatum->v1 & mask)){
 		connected = true;
 	}	
  ASSERT_EQ(connected,true);
}