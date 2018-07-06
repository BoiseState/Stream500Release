// Copyright (c) 2018 Boise State University 
// All rights reserved.

// Developed by: 	    Stream500 Team
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

#include "SendObj.h"

/******************************************************************************
 * Goal:
 * Constructor for the SendObj
*******************************************************************************/
SendObj::SendObj(){}

/******************************************************************************
 * Parameters:
 * num_bytes - The number of bytes in the message to be sent
 * msg_buf - The buffer containing the message to be sent
 * Goal:
 * Package the message into a packet to be sent to the Responder
*******************************************************************************/
uint64_t SendObj::send_edge(int32_t num_bytes, void *msg_buf){}

/******************************************************************************
 * Parameters:
 * num_bytes - The number of bytes in the message to be sent
 * msg_buf - The buffer containing the message to be sent
 * q_priority - The priority level of the query being sent
 * query_id - The ID of the query being sent
 * Goal:
 * Package the message into a packet to be sent to the Responder
*******************************************************************************/
uint64_t SendObj::send_query(int32_t num_bytes, void *msg_buf, int q_priority, uint64_t query_id){}

/******************************************************************************
 * Goal:
 * Return the number of packets sent at time of call
*******************************************************************************/
uint64_t SendObj::get_num_packets_sent(){}

/******************************************************************************
 * Parameters:
 * gen_num - Number of generator sending DONE packet
 * Goal:
 * Send packets to the Responder signifying that the generation process has
 * finished on the designated generator
*******************************************************************************/
void SendObj::send_done(int gen_num){}

/******************************************************************************
 * Goal:
 * Destructor for the SendObj
*******************************************************************************/
SendObj::~SendObj(){}
