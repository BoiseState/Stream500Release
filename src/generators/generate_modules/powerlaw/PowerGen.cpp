// Copyright (c) 2018 Boise State University 
// All rights reserved.

// Developed by:     Stream500 Team
//                   Boise State University
//                   https://github.com/BoiseState/Stream500

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

#include "PowerGen.h"

PowerGen::PowerGen(int init_num_nodes, double init_concentration, int init_rand_max, unsigned int init_kseed)
{
  class_id = 0;
  num_nodes = init_num_nodes;
  concentration = init_concentration;
  kseed = init_kseed;

  power = power_law_initialize(concentration, num_nodes, init_rand_max);
}

void PowerGen::generate_edge(int width, int offset, edge *e)
{
  uint64_t rn = rand_r(&kseed);
  uint64_t vertex0;
  uint64_t vertex1;
  while ((vertex0 = power_law_simulate(rn,power)) >= num_nodes){
    rn = rand_r(&kseed);
  }
  vertex1 = rand_r(&kseed) % num_nodes;

  ensure_same_subgraph(width, offset, &vertex0, &vertex1, 0);

  e->v0 = vertex0;
  e->v1 = vertex1;
}

PowerGen::~PowerGen()
{
}
