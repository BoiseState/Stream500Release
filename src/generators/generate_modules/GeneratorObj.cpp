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

#include "GeneratorObj.h"

GeneratorObj::GeneratorObj(){}

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
void GeneratorObj::ensure_same_subgraph(int width, int offset, uint64_t *v0,
                                   uint64_t *v1, int has_been_switched)
{
  uint64_t original_v0 = *v0;
  uint64_t original_v1 = *v1;
    
  uint64_t mask_base = 0;
  // set the correct number of bits to 1
  for (int i = 0; i < width; i++)
  {
    mask_base |= 1 << i;
  }
  // shift the bits to be in the correct location
  uint64_t mask = mask_base << offset;

  // checking to see if the masks already match
  if ((*v0 & mask) != (*v1 & mask))
  {
    // capture the bits that are not covered by the mask in v1 
    // the bits in the mask area are all 0
    uint64_t neg_mask = ~mask;
    *v1 &= neg_mask;
    // capture the bits only in the mask area of v0
    // and then when we OR them we have v1 with v0's mask 
    *v1 |= (*v0 & mask);
  }  
  // check the value of v1, if it is out of range - call this function
  // again and switch v0 and v1
  if (*v1 >= num_nodes && !has_been_switched)
  {
    printf("Pushed out of range!! Values - v0: %ld\tv1: %ld", original_v0, original_v1);
    *v0 = original_v0;
    *v1 = original_v1;
    ensure_same_subgraph(width, offset, v1, v0, 1);
  }
}

/******************************************************************************
 * Parameters:
 * width - describes the subgraph mask in bits
 * offset - number of bits from the least significant, starts count at 0
 * v0 - the first vertex
 * v1 - second vertex
 * has_been_switched - a flag to ensure a given pair of vertices is only
 * processed, at max, two times.
 * Goal:
 * Make sure the subgraph masks are different, so that they are not allowed to
 * be connected to one another
 *
 * If they have the same subgraph mask, set v1's subgraph mask to the negated
 * subgraph mask of v0
*******************************************************************************/
void GeneratorObj::ensure_different_subgraph(int width, int offset, uint64_t *v0, 
					uint64_t *v1, int has_been_switched)
{
  uint64_t original_v0 = *v0;
  uint64_t original_v1 = *v1;
    
  uint64_t mask_base = 0;
  for (int i = 0; i < width; i++)
  {
    mask_base |= 1 << i;
  }
  uint64_t mask = mask_base << offset;

  if ((*v0 & mask) == (*v1 & mask))
  {
    uint64_t neg_mask = ~mask;
    *v1 &= neg_mask;
    *v1 |= (~*v0 & mask);
  }

  // check the value of v1, if it is out of range - call this function
  // again and switch v0 and v1
  if (*v1 >= num_nodes && !has_been_switched)
  {
    *v0 = original_v0;
    *v1 = original_v1;
    ensure_different_subgraph(width, offset, v1, v0, 1);
  }
}

/******************************************************************************
 * Parameters:
 * width - describes the subgraph mask in bits
 * offset - number of bits from the least significant, starts count at 0
 * e - An edge pointer for writing the vertex ID's of an edge
 * Goal:
 * Generate two vertex ID's, according to some mechanism, that constitute a
 * valid edge
*******************************************************************************/
void GeneratorObj::generate_edge(int width, int offset, edge *e){}

/******************************************************************************
 * Parameters:
 * width - describes the subgraph mask in bits
 * offset - number of bits from the least significant, starts count at 0
 * path_length - The number of edges to creat in the path between the two
 * vertex ID's of the query
 * path - A pointer to a collection of edges for storing the path
 * q - A pointer to a query for storing the created query
 * Goal:
 * Generate two vertex ID's that have matching subgraph mask values and a
 * collection of edges that create a continuous path between the two vertex ID's.
 * Store the collection of edges in the path variable, and the two vertex ID's
 * of interest in the q variable, along with a unique ID for the query.
*******************************************************************************/
void GeneratorObj::generate_yes(int width, int offset, int path_length, edge **path, query *q)
{
  std::uniform_int_distribution<int> coin_flip(0,1);

  generate_edge(width, offset, path[0]);		//Use generate edge to create first vertex in path
  generate_edge(width, offset, path[path_length - 1]);	//Do same for second to last vertex in path

  ensure_same_subgraph(width, offset, &path[0]->v0, &path[path_length - 1]->v0, 0);
  ensure_same_subgraph(width, offset, &path[path_length - 1]->v0, &path[path_length - 1]->v1, 0);

  for (int i = 1; i < path_length - 1; i++)
  {
    generate_edge(width, offset, path[i]);
    ensure_same_subgraph(width, offset, &path[i-1]->v0, &path[i]->v0, 0);
    ensure_same_subgraph(width, offset, &path[i]->v0, &path[i]->v1, 0);
    // NOTE: Can maybe do without the coinflip...
    if (coin_flip(rand_generator))
    {
      path[i]->v0 = path[i-1]->v0;      
    }
    else
    {
      path[i]->v0 = path[i-1]->v1;
    }
  }
  path[path_length - 2]->v1 = path[path_length - 1]->v0;
  q->v0 = path[0]->v0;
  q->v1 = path[path_length - 1]->v1;
  q->id = class_id;
  class_id++;
}

/******************************************************************************
 * Parameters:
 * width - describes the subgraph mask in bits
 * offset - number of bits from the least significant, starts count at 0
 * q - A query pointer for storing the ID's of a generated query
 * Goal:
 * Generate two vertex ID's that have different subgraph mask values and store
 * in a query pointer, along with a unique ID identifying this query.
*******************************************************************************/
void GeneratorObj::generate_no(int width, int offset, query *q)
{
  //NOTE: Randomly generating numbers since generating an edge here that isn't counted
  //in generator_engine currently leads to issues for Kronecker generator
  //running out of memory allocated to it in packed_edge pointer.
  //Possible fix would be to remove reliance on packed_edge pointer in Kronecker generator.
  edge *e = (edge *)malloc(sizeof(edge));
  generate_edge(width, offset, e);
  ensure_different_subgraph(width, offset, &e->v0, &e->v1, 0);
  q->v0 = e->v0;
  q->v1 = e->v1;
/*
  int64_t v0 = rand() % num_nodes;
  int64_t v1 = rand() % num_nodes;
  ensure_different_subgraph(width, offset, &v0, &v1, 0);
  q->v0 = v0;
  q->v1 = v1;
*/
  q->id = class_id;
  class_id++;
  free(e);
}

/******************************************************************************
 * Goal:
 * Return the current value of the class variable, class_id
*******************************************************************************/
uint64_t GeneratorObj::get_current_query_id()
{
  return class_id;
}

/******************************************************************************
 * Goal:
 * Destructor for the GeneratorObj class
*******************************************************************************/
GeneratorObj::~GeneratorObj(){}
