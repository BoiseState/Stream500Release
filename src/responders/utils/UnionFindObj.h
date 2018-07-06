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

#ifndef UNION_FIND_OBJ_H
#define UNION_FIND_OBJ_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

class UnionFindObj
{
  private:
    uint64_t *id;
    uint64_t *sz;

  public:
    UnionFindObj(uint64_t num_nodes);
    uint64_t get_parent(uint64_t vert_id);
    void set_parent(uint64_t vert_id, uint64_t new_parent_id);
    uint64_t root(uint64_t vert_id);
//    int find(uint64_t p, uint64_t q);
//    void unite(uint64_t p, uint64_t q);
    uint64_t find(uint64_t p);
    void unite(uint64_t r_p, uint64_t r_q);
    ~UnionFindObj();
};

#endif //UNION_FIND_OBJ_H

