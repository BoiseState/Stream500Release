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

#include "RespSetupMethods.h"

/******************************************************************************
 * Parameters:
 * resp_init - The collection of parameters to be used for the run.
 * Goal:
 * Using the parameter values from the resp_eng_init object, setup the ParseObj
 * to be uses for the run.
*******************************************************************************/
ParseObj* setup_parser(resp_eng_init* resp_init)
{
  ParseObj *parser;
  switch(resp_init->get_parse_type())
  {
    case 0:             //Parse Message containing only vertex ID's as 64-bit integers
      parser = new ParseObj();
      break;
    case 1:             //Parse Message containing "junk" text and usernames as text
      printf("Using Semi-Twitter Obfuscator\n");
      parser = new TwitterParser();
      break;
    default:
    {
      parser = NULL;
      perror("Unsupported parser type\n");
      exit(1);
    }
  }
  return parser;
}


/******************************************************************************
 * Parameters:
 * resp_init - The collection of parameters to be used for the run.
 * Goal:
 * Using the parameter values from the resp_eng_init object, setup the RecvObj
 * to be uses for the run.
*******************************************************************************/
RecvObj* setup_receiver(resp_eng_init* resp_init)
{
  RecvObj *rcvr;
  switch(resp_init->get_receive_type())
  {
    case 0:             //UDP socket
      rcvr = new UdpRecv(resp_init->get_to_resp_port());
      break;
    default:
    {
      rcvr = NULL;
      perror("Unsupported receiver type\n");
      exit(1);
    }
  }
  return rcvr;
}

