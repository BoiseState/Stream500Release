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

#include "gtest/gtest.h"
#include "../data_types.h"
#include "ResultsBuffer.h"


/*****************************************************************************************
 * Goal:
 * Test class to test the functionality of ResultBuffer.java class
******************************************************************************************/
class ResultsBufferTest : public testing::Test {
  protected:  
    //Variables
    size_t buffer_size; //size of circular buffer is (buffer_size + 1)
    ResultsBuffer *result_buf_obj;
    uint64_t result, q_id;
    int c_count, w_yes_count, w_no_count, dropped_query_count;

    //constructor
    ResultsBufferTest(){
      buffer_size = 5;    
      result_buf_obj = new ResultsBuffer(buffer_size);  
    }
    //destructor
    virtual ~ResultsBufferTest(){
      delete result_buf_obj;
    }
};


/*****************************************************************************************
 * Goal:
 * To test that the buffer becomes full when the size of buffer reaches (buffer_size + 1)
******************************************************************************************/
TEST_F(ResultsBufferTest, buffer_full_and_query_dropped_test) { 
  	//Variables
	dropped_query_count = 0;
	
    //Add records to the circular buffer
	result_buf_obj->add_and_check_for_dropped(1, 0, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(1, 1, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(0, 2, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(1, 3, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(0, 4, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(0, 5, &dropped_query_count);	
	//dropped query count should be zero at the beginning
	ASSERT_EQ(dropped_query_count, 0); 

	//At this point the circular buffer is full. Adding one record to it must cause one dropped query count
	result_buf_obj->add_and_check_for_dropped(0, 6, &dropped_query_count);
	ASSERT_EQ(dropped_query_count, 1);  

	//Add one more and there should be 2 dropped query count
	result_buf_obj->add_and_check_for_dropped(0, 7, &dropped_query_count);
	ASSERT_EQ(dropped_query_count, 2);
}


/*****************************************************************************************
 * Goal:
 * To Test that the dropped query count are correctly recorded by the final finish function
******************************************************************************************/
TEST_F(ResultsBufferTest, dropped_query_count_test) { 
  	//Variables
	dropped_query_count = 0;
	
    //Add records to the circular buffer
	result_buf_obj->add_and_check_for_dropped(1, 0, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(1, 1, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(0, 2, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(1, 3, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(0, 4, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(0, 5, &dropped_query_count);	
	//dropped query count should be zero at the beginning
	ASSERT_EQ(dropped_query_count, 0); 

	//At this point none of the queries has been answered.
	//Finish function takes all unswered queries as dropped.
	result_buf_obj->finish(&dropped_query_count);
	ASSERT_EQ(dropped_query_count, 6);
}


/*****************************************************************************************
 * Goal:
 * To Test that the correct query count are correctly recorded
******************************************************************************************/
TEST_F(ResultsBufferTest, correct_query_count_test) { 
	//Variables
	dropped_query_count = 0;
	c_count = 0;

	//Add records to the circular buffer
	result_buf_obj->add_and_check_for_dropped(1, 0, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(1, 1, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(0, 2, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(1, 3, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(0, 4, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(0, 5, &dropped_query_count);	

	//Let's answer query with id 0 and 1 correctly
	result_buf_obj->search_and_update_stats(1, 0, &c_count,  &w_yes_count,  &w_no_count);
	result_buf_obj->search_and_update_stats(1, 1, &c_count,  &w_yes_count,  &w_no_count);
	//At this point the total correct_query_count should be 2.
	ASSERT_EQ(c_count, 2);

	//Let's answer the query id 3 incorrectly. 
	result_buf_obj->search_and_update_stats(1, 2, &c_count,  &w_yes_count,  &w_no_count);
	ASSERT_EQ(c_count, 2);

	//Now, let's anser the query id 5 correctly.
	result_buf_obj->search_and_update_stats(0, 5, &c_count,  &w_yes_count,  &w_no_count);
	ASSERT_EQ(c_count, 3);

	//Now, let's anser the query id 3 correctly.
	result_buf_obj->search_and_update_stats(1, 3, &c_count,  &w_yes_count,  &w_no_count);
	ASSERT_EQ(c_count, 4);
}


/*****************************************************************************************
 * Goal:
 * To Test that the wrong yes query count are correctly recorded
******************************************************************************************/
TEST_F(ResultsBufferTest, wrong_yes_query_count_test) { 
	dropped_query_count = 0;
	w_yes_count = 0;

    //Add only yes queries (i.e. indicated by result = 1)
	result_buf_obj->add_and_check_for_dropped(1, 0, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(1, 1, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(1, 2, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(1, 3, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(1, 4, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(1, 5, &dropped_query_count);	

	//Let's answer query with id 0 and 1 correctly
	result_buf_obj->search_and_update_stats(1, 0, &c_count,  &w_yes_count,  &w_no_count);
	result_buf_obj->search_and_update_stats(1, 1, &c_count,  &w_yes_count,  &w_no_count);
	//At this point the total correct_query_count should be 2.
	ASSERT_EQ(w_yes_count, 0);

	//Let's answer the query id 2 incorrectly. 
	result_buf_obj->search_and_update_stats(0, 2, &c_count,  &w_yes_count,  &w_no_count);
	ASSERT_EQ(w_yes_count, 1);

	//Now, let's anser the query id 4 and 5 incorrectly.
	result_buf_obj->search_and_update_stats(0, 4, &c_count,  &w_yes_count,  &w_no_count);
	ASSERT_EQ(w_yes_count, 2);
	result_buf_obj->search_and_update_stats(0, 5, &c_count,  &w_yes_count,  &w_no_count);
	ASSERT_EQ(w_yes_count, 3);

	// Now, let's anser the query id 3 correctly
	result_buf_obj->search_and_update_stats(1, 3, &c_count,  &w_yes_count,  &w_no_count);
	ASSERT_EQ(w_yes_count, 3);
}


/*****************************************************************************************
 * Goal:
 * To Test that the wrong no query count are correctly recorded
******************************************************************************************/
TEST_F(ResultsBufferTest, wrong_no_query_count_test) { 
	dropped_query_count = 0;
	w_no_count = 0;

    //Add only no queries (i.e. indicated by result = 0)
	result_buf_obj->add_and_check_for_dropped(0, 0, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(0, 1, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(0, 2, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(0, 3, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(0, 4, &dropped_query_count);
	result_buf_obj->add_and_check_for_dropped(0, 5, &dropped_query_count);	

	//Let's answer query with id 0 and 1 correctly
	result_buf_obj->search_and_update_stats(0, 0, &c_count,  &w_yes_count,  &w_no_count);
	result_buf_obj->search_and_update_stats(0, 1, &c_count,  &w_yes_count,  &w_no_count);

	//At this point the total wrong_query_count should be 0.
	ASSERT_EQ(w_no_count, 0);

	//Let's answer the query id 2 incorrectly. 
	result_buf_obj->search_and_update_stats(1, 2, &c_count,  &w_yes_count,  &w_no_count);
	ASSERT_EQ(w_no_count, 1);

	//Now, let's anser the query id 4 and 5 incorrectly.
	result_buf_obj->search_and_update_stats(1, 4, &c_count,  &w_yes_count,  &w_no_count);
	ASSERT_EQ(w_no_count, 2);
	result_buf_obj->search_and_update_stats(1, 5, &c_count,  &w_yes_count,  &w_no_count);
	ASSERT_EQ(w_no_count, 3);

	// Now, let's anser the query id 3 correctly
	result_buf_obj->search_and_update_stats(0, 3, &c_count,  &w_yes_count,  &w_no_count);
	ASSERT_EQ(w_no_count, 3);
}