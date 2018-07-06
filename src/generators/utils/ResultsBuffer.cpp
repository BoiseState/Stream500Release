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

#include "ResultsBuffer.h"

/******************************************************************************
 * Parameters:
 * size - The max number of query results the circular buffer can hold
 * Goal:
 * Initialize the buffer
*******************************************************************************/
ResultsBuffer::ResultsBuffer(size_t size) :
      buf_(std::unique_ptr<query_result[]>(new query_result[size + 1])),
      size_(size + 1)
{
  pthread_rwlock_init(&lock, NULL);
  int i;
  for (i = 0; i < size_; i++)
  {
    buf_[i].checked = 1;
  }
}

/******************************************************************************
 * Parameters:
 * result - The expected result of a new query
 * q_id - The id of a new query
 * d_count - Pointer to the dropped query counter
 * Goal:
 * Add new query result to the circular buffer and check if the result pointed 
 * to by the head pointer was dropped (not checked).
*******************************************************************************/
void ResultsBuffer::add_and_check_for_dropped(uint64_t result, uint64_t q_id, int *d_count)
{
  pthread_rwlock_wrlock(&lock);
  if (!buf_[head_].checked)
  {
    (*d_count)++;
    num_elements--;
  }

  buf_[head_].result = result;
  buf_[head_].id = q_id;
  buf_[head_].checked = 0;
  num_elements++;
  head_ = (head_ + 1) % size_;

  if(head_ == tail_)
  {
    tail_ = (tail_ + 1) % size_;
  }

  pthread_rwlock_unlock(&lock);
}

/******************************************************************************
 * Parameters:
 * result - The result received for a query from the Responder
 * q_id - The id for a query from the Responder
 * c_count - Pointer to the correct query counter
 * w_yes_count - Pointer to the wrong-yes query counter
 * w_no_count - Pointer to the wrong-no query counter
 * Goal:
 * Given a query result (from the Responder), search for the query in the buffer
 * and update counter values based on whether the result matches the expected
 * result. (Queries may not be found in the buffer if the buffer becomes full
 * and the query is overwritten by new query results from the generator)
*******************************************************************************/
void ResultsBuffer::search_and_update_stats(uint64_t result, uint64_t q_id, int *c_count, 
					int *w_yes_count, int *w_no_count)
{
  pthread_rwlock_rdlock(&lock);
  int iter = tail_;
  while (iter != (tail_ + size_ - 1) % size_)
  {
    if (buf_[iter].checked || q_id != buf_[iter].id)
    {
      iter = (iter + 1) % size_;
      continue;
    }
    else if (q_id == buf_[iter].id)
    {
      break;
    }
  }
  if (q_id == buf_[iter].id)
  {
    if (result == buf_[iter].result)
    {
      (*c_count)++;
    }
    else
    {
      if (buf_[iter].result)
      {
        (*w_yes_count)++;
      }
      else
      {
        (*w_no_count)++;
      }
    }
    buf_[iter].checked = 1;
    num_elements--;
  }
  else
  {
    printf("Didn't find it... %lu\tnumElements:\t%lu\n", q_id, num_elements);
  }
  // Push tail pointer forward past any query results that are marked checked
  while (buf_[tail_].checked && tail_ != head_)
  {
    tail_ = (tail_ + 1) % size_;
  }
  pthread_rwlock_unlock(&lock);
}

/******************************************************************************
 * Parameters:
 * d_count - Pointer to dropped query counter
 * Goal:
 * Check every slot of the buffer for any query results not marked as checked
 * and increment dropped query count for any found.
*******************************************************************************/
void ResultsBuffer::finish(int *d_count)
{
  for (tail_ = 0; tail_ < size_; tail_++)
  {
    if (!buf_[tail_].checked)
    {
      (*d_count)++;
      buf_[tail_].checked = 1;
    }
  }
  pthread_rwlock_destroy(&lock);
}

