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

#ifndef RESULTS_BUFFER_H
#define RESULTS_BUFFER_H

#include "../data_types.h"
#include <pthread.h>
#include <cstdio>

#include <memory>
#include <mutex>

class ResultsBuffer {
  private:
    pthread_rwlock_t lock;
    std::unique_ptr<query_result[]> buf_;
    size_t num_elements = 0;
    size_t head_ = 0;
    size_t tail_ = 0;
    size_t size_;

  public:
    ResultsBuffer(size_t size);
    void add_and_check_for_dropped(uint64_t result, uint64_t q_id, int *d_count);
    void search_and_update_stats(uint64_t result, uint64_t q_id, int *c_count, int *w_yes_count, int *w_no_count);
    void finish(int *d_count);
};

#endif //RESULTS_BUFFER_H
