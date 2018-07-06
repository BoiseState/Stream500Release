// Copyright (c) 2018 Boise State University 
// All rights reserved.

// Developed by:      Stream500 Team
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

#include "GenSetupMethods.h"
#include <iostream>

/******************************************************************************
 * Parameters:
 * cp - The Configuration object that stores all parameters for a run.
 * Goal:
 * Based on the parameter values in the Configuration, setup the Generator
 * Object that generates all edges for a run.
*******************************************************************************/
GeneratorObj* setup_generator(ConfigParser* cp)
{
  GeneratorObj *generator;
  switch(cp->get_gen_type())
  {
    case 0:             //Kronecker - Graph500
    {
      printf("Using Kronecker Generator\n");
      uint_fast32_t seed[5];
      make_mrg_seed(cp->get_mrg_seed_1(), cp->get_mrg_seed_2(), seed);
      generator = new KronGen(seed, cp->get_log_num_nodes(), 0, (int64_t)cp->get_edge_factor() 
				<< cp->get_log_num_nodes());
      break;
    }
    case 1:             //Erdos-Renyi
    {
      //double erd_ren_prob = 0.5;
      printf("Using Erdos-Renyi Generator\n");
      generator = new ErdRenGen((int64_t)1 << cp->get_log_num_nodes(), (int64_t)cp->get_edge_factor() 
				<< cp->get_log_num_nodes(), cp->get_rand_seed());
      break;
    }
    case 2:             //Power Law - Firehose
    {
      printf("Using Power Law Generator\n");
      generator = new PowerGen((int64_t)1 << cp->get_log_num_nodes(), cp->get_concentration(), 
				RAND_MAX, cp->get_kseed());
      break;
    }

    // case 3:             //From File - semi-colon delineated edges
    // {
    //   generator = new FromFileGen((int64_t)1 << cp->get_log_num_nodes(), file_name);
    //   std::ifstream in_file;
    //   in_file.open(file_name);
    //   //num_edges = std::count(std::istreambuf_iterator<char>(in_file), std::istreambuf_iterator<char>(), '\n');
    //   in_file.close();
    //   break;
    // }

    default:
    {
      generator = NULL;
      printf("Unsupported generator type\n");
      exit(1);
    }
  }
  return generator;
}

/******************************************************************************
 * Parameters:
 * cp - The Configuration object that stores all parameters for a run.
 * Goal:
 * Based on the parameter values in the Configuration, setup the Obfuscator
 * Object that is used to obfuscate edges/queries.
*******************************************************************************/
ObfuscateObj* setup_obfuscator(ConfigParser* cp)
{
  ObfuscateObj *obfuscator;
  switch(cp->get_obf_type())
  {
    case 0:             //No obfuscation
      obfuscator = new ObfuscateObj();      
      break;
    case 1:             //Twitter obfuscation
      printf("Using Semi-Twitter Obfuscator\n");
      obfuscator = new TwitterObfuscator();
      break;
    default:
    {
      obfuscator = NULL;
      printf("Unsupported obfuscator type\n");
      exit(1);
    }
  }
  return obfuscator;
}

/******************************************************************************
 * Parameters:
 * cp - The Configuration object that stores all parameters for a run.
 * Goal:
 * Based on the parameter values in the Configuration, setup the Sender
 * Object that is used to send edges/queries.
*******************************************************************************/
SendObj* setup_sender(ConfigParser* cp)
{
  SendObj *sender;
  switch(cp->get_send_type())
  {
    case 0:             //UDP socket
    {
      sender = new UdpSend(cp->get_max_msg_len(), cp->get_per_packet(),
                        cp->get_ip_address(), cp->get_to_resp_port());
      break;
    }
    default:
    {
      sender = NULL;
      printf("Unsupported sender type\n");
      exit(1);
    }
  }
  return sender;
}

