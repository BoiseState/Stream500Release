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

#include "UnionFindObj.h"

/******************************************************************************
 * Parameters:
 * num_nodes - The number of nodes in the graph
 * Goal:
 * Constructor for the UnionFindObj class.
 *
 * Given a size, allocates memory and initializes values for the data structures
 * used for tracking UnionFind information.
*******************************************************************************/
UnionFindObj::UnionFindObj(uint64_t num_nodes)
{
  id = (uint64_t *)malloc((uint64_t)num_nodes*sizeof(uint64_t));
  sz = (uint64_t *)malloc((uint64_t)num_nodes*sizeof(uint64_t));
  if (id == NULL){
    printf("id array didn't get allocated\n");
    exit(1);
  } 
  if (sz == NULL){
    printf("sz array didn't get allocated\n");
    exit(1);
  }

  for (uint64_t i = 0; i < num_nodes; i++)
  {
    id[i] = i;
    sz[i] = 1;
  }
}

/******************************************************************************
 * Parameters:
 * vert_id - The ID of the vertex of interest
 * Goal:
 * Return the ID of the parent of the vertex of interest
*******************************************************************************/
uint64_t UnionFindObj::get_parent(uint64_t vert_id)
{
  return id[vert_id];
}

/******************************************************************************
 * Parameters:
 * vert_id - The ID of the vertex of interest
 * new_parent_id - The ID to be set for the parent of the vertex of interest
 * Goal:
 * Set the ID of the parent of the vertex of interest
*******************************************************************************/
void UnionFindObj::set_parent(uint64_t vert_id, uint64_t new_parent_id)
{
  id[vert_id] = new_parent_id;
}

/******************************************************************************
 * Parameters:
 * vert_id - The vertex of interest
 * Goal:
 * Return the ID of the root of the vertex of interest
 *
 * Also performs path compression to maintain shallow depth of spanning trees.
*******************************************************************************/
uint64_t UnionFindObj::root(uint64_t vert_id)
{
  uint64_t original_node = vert_id;
  uint64_t root;
  uint64_t next;

  //Find root of tree that vertex belongs to
  uint64_t temp;
  while (vert_id != (temp=get_parent(vert_id)))
  {
    set_parent(vert_id, get_parent(get_parent(vert_id)));
    vert_id = temp;
  }

//  root = vert_id;
//  vert_id = original_node;
/*
  //Perform path compression
  while (vert_id != root)
  {
    next = get_parent(vert_id);
    set_parent(vert_id, root);
    vert_id = next;
  }
//  return root;
*/
  return vert_id;
}
/*
int UnionFindObj::find(uint64_t p, uint64_t q)
{
  return root(p) == root(q);
}
*/

/******************************************************************************
 * Parameters:
 * p - The vertex of interest
 * Goal:
 * Returns the root ID of the vertex of interest
*******************************************************************************/
uint64_t UnionFindObj::find(uint64_t p)
{
  return root(p);
}

/*
void UnionFindObj::unite(uint64_t p, uint64_t q)
{
  uint64_t i = root(p);
  uint64_t j = root(q);

  //Perform weight comparison
  if (sz[i] < sz[j])
  {
    id[i] = j;
    sz[j] += sz[i];
  }
  else
  {
    id[j] = i;
    sz[i] += sz[j];
  }
}
*/

/******************************************************************************
 * Parameters:
 * p - First of two ID's for vertices of interest
 * q - Second of two ID's for vertices of interest
 * r_p - The ID for the root of the first vertex of interest
 * r_q - The ID for the root of the second vertex of interest
 * Goal:
 * Connect the roots of two previously disconnected spanning trees.
 *
 * Given ID's of the roots of two different spanning trees, make the root
 * with the smaller associated spanning tree a child of the other root and
 * update size values accordingly.
*******************************************************************************/
void UnionFindObj::unite(uint64_t r_p, uint64_t r_q)
{
  //Perform weight comparison
  if (sz[r_p] < sz[r_q])
  {
    id[r_p] = r_q;
    sz[r_q] += sz[r_p];
  }
  else
  {
    id[r_q] = r_p;
    sz[r_p] += sz[r_q];
  }
}

/******************************************************************************
 * Goal:
 * Destructor for the UnionFindObj class
*******************************************************************************/
UnionFindObj::~UnionFindObj()
{
  free(id);
  free(sz);
}
