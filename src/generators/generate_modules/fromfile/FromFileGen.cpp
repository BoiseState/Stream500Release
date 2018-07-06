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

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "FromFileGen.h"

FromFileGen::FromFileGen(int init_num_nodes, char *file_name)
{
  num_nodes = init_num_nodes;
  class_id = 0;
  input_file.open(file_name);
}

void FromFileGen::generate_edge(int width, int offset, edge *e)
{
  std::string line;
  std::getline(input_file, line, ';');
  int v0 = stoi(line, nullptr);
  std::getline(input_file, line, '\n');
  int v1 = stoi(line, nullptr);
  if ((v0 % 2) != (v1 % 2))
  {
    v1 += 1;
  }
  e->v0 = v0 % num_nodes;
  e->v1 = v1 % num_nodes;
}

FromFileGen::~FromFileGen()
{

}
