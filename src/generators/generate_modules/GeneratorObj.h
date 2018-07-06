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

#ifndef QUERY_GENERATOR_H
#define QUERY_GENERATOR_H

#include <stdlib.h>
#include <stdint.h>
#include <random>
#include "../data_types.h"

class GeneratorObj
{
  protected:
    std::default_random_engine rand_generator;
    uint64_t num_nodes;
    uint64_t class_id;

  public:
    GeneratorObj();
    virtual void generate_edge(int width, int offset, edge *e);
    void generate_yes(int width, int offset, int path_length, edge **path, query *q);
    void generate_no(int width, int offset, query *q);
    uint64_t get_current_query_id();
    void ensure_same_subgraph(int width, int offset, uint64_t *v0, uint64_t *v1, int has_been_switched);
    void ensure_different_subgraph(int width, int offset, uint64_t *v0, uint64_t *v1, int has_been_switched);
    virtual ~GeneratorObj();
};

#endif  //QUERY_GENERATOR_H
