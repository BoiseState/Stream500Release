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
#include "UnionFindObj.h"
#include "gtest/gtest.h"
#include <getopt.h>

/*****************************************************************************************
 * Goal:
 * Test class to test the functionality of UnionFindObj
******************************************************************************************/
class UnionFindObjTest : public testing::Test {
  protected:
    uint64_t numNodes;
    UnionFindObj* UFTest;

    //constructor
    UnionFindObjTest(){
      numNodes = 10;
      UFTest = new UnionFindObj(numNodes);
    }

    //destructor
    virtual ~UnionFindObjTest(){
      delete UFTest;
    }
};  


/*****************************************************************************************
 * Goal:
 * Positive Testing for testing that parentValue is set properly
******************************************************************************************/
TEST_F(UnionFindObjTest, parentEQTest) { 
  uint64_t parentValue = numNodes-1;
  uint64_t childValue = 5;
  UFTest->set_parent(childValue,parentValue);
  ASSERT_EQ(UFTest->get_parent(childValue),parentValue);
}


/*****************************************************************************************
 * Goal:
 * Negative testing to test that the parentValue is not set properly
******************************************************************************************/
TEST_F(UnionFindObjTest, parentUNEQTest) {
  uint64_t parentValue = numNodes-1;
  uint64_t childValue = 5;
  UFTest->set_parent(childValue,parentValue);
  ASSERT_NE(UFTest->get_parent(parentValue),childValue);
}


/*****************************************************************************************
 * Goal:
 * Test the root method
******************************************************************************************/
TEST_F(UnionFindObjTest, rootTest) {
  uint64_t parentValue = 3;
  uint64_t midValue1 = 7;
  uint64_t midValue2 = 4;
  uint64_t childValue1 = 5;
  uint64_t childValue2 = 9;
  uint64_t falseValue = 2;

  UFTest->unite(UFTest->root(parentValue),UFTest->root(midValue1));
  UFTest->unite(UFTest->root(midValue1),UFTest->root(childValue1));
  UFTest->unite(UFTest->root(midValue2),UFTest->root(childValue2));
  UFTest->unite(UFTest->root(midValue2),UFTest->root(parentValue));

  ASSERT_EQ(UFTest->root(childValue1),UFTest->root(childValue2));
  ASSERT_EQ(UFTest->root(midValue1),UFTest->root(midValue2));
  ASSERT_EQ(UFTest->root(childValue1),UFTest->root(midValue1));
  ASSERT_EQ(UFTest->root(childValue2),UFTest->root(midValue2));
  ASSERT_EQ(UFTest->root(childValue1),parentValue);
  ASSERT_EQ(UFTest->root(midValue1),parentValue);
  ASSERT_EQ(UFTest->root(parentValue),parentValue);

  ASSERT_NE(UFTest->root(childValue1),falseValue);
  ASSERT_NE(UFTest->root(midValue1),falseValue);
  ASSERT_NE(UFTest->root(parentValue),falseValue);
}


/*****************************************************************************************
 * Goal:
 * Test the find method
******************************************************************************************/
TEST_F(UnionFindObjTest, findTest) {
  uint64_t parentValue = 7;
  uint64_t midValue1 = 9;
  uint64_t midValue2 = 1;
  uint64_t childValue1 = 2;
  uint64_t childValue2 = 5;
  uint64_t falseValue = 4;

  UFTest->unite(UFTest->root(childValue1),UFTest->root(midValue1));
  UFTest->unite(UFTest->root(childValue2),UFTest->root(midValue2));
  UFTest->unite(UFTest->root(midValue1),UFTest->root(parentValue));
  UFTest->unite(UFTest->root(midValue2),UFTest->root(parentValue));

  ASSERT_EQ(UFTest->find(midValue1),UFTest->find(midValue2));
  ASSERT_EQ(UFTest->find(childValue1),UFTest->find(childValue2));
  ASSERT_EQ(UFTest->find(midValue1),UFTest->find(childValue1));
  ASSERT_EQ(UFTest->find(midValue2),UFTest->find(childValue2));
  ASSERT_EQ(UFTest->find(childValue1),UFTest->find(midValue2));
  ASSERT_EQ(UFTest->find(childValue2),UFTest->find(midValue1)); 

  ASSERT_NE(UFTest->find(falseValue),UFTest->find(childValue1));
  ASSERT_NE(UFTest->find(falseValue),UFTest->find(childValue2));
  ASSERT_NE(UFTest->find(falseValue),UFTest->find(midValue1));
  ASSERT_NE(UFTest->find(falseValue),UFTest->find(midValue2));
  ASSERT_NE(UFTest->find(falseValue),UFTest->find(parentValue));
}



/*****************************************************************************************
 * Goal:
 * Test the unite method
******************************************************************************************/
TEST_F(UnionFindObjTest, uniteTest) {
  uint64_t childValue1 = 9;
  uint64_t childValue2 = 0;
  uint64_t midValue1 = 5;
  uint64_t midValue2 = 7;
  uint64_t parentValue = 4;
  uint64_t extraValue = 6;

  UFTest->unite(UFTest->root(parentValue),UFTest->root(midValue1));
  UFTest->unite(UFTest->root(parentValue),UFTest->root(childValue1));
  UFTest->unite(UFTest->root(midValue2),UFTest->root(childValue2));
  UFTest->unite(UFTest->root(childValue1),UFTest->root(childValue2));

  ASSERT_EQ(UFTest->root(childValue1),UFTest->root(childValue2));
  ASSERT_EQ(UFTest->get_parent(midValue2),parentValue);
  ASSERT_EQ(UFTest->get_parent(childValue2),parentValue);
  ASSERT_NE(UFTest->get_parent(midValue1),midValue2);

  UFTest->unite(extraValue,midValue2);
  ASSERT_EQ(UFTest->root(extraValue),UFTest->root(midValue2));
  ASSERT_EQ(UFTest->get_parent(extraValue),parentValue);
}
