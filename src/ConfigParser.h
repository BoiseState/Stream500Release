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

#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <fstream>
#include <sstream>
#include <stdint.h>
#include <unistd.h>

class ConfigParser 
{
  private:
    //If parameters were in a map, much of the functionality would be simplified

    // General graph description
    int log_num_nodes;
    int edge_factor;

    // Parameters for generator object selection
    int gen_type;
    int obf_type;
    int send_type;

    // Parameters for responder object selection
    int parse_type;
    int receive_type;

    // Parameters for network communication
    uint16_t to_resp_port;
    uint16_t to_gen_port;
    char *ip_address;

    // General parameters for generation behavior
    int gen_rate;
    int output_interval;
    int per_packet;
    int result_buf_size;
    int mask_width;
    int mask_offset;
    int max_msg_len;
    int max_uname_size;
    int num_gen;

    // Parameters for general random number generators
    int rand_seed;
    int min_length;
    int max_length;

    // Probabilities for generation behavior
    double gen_edge_prob;
    double gen_yes_prob;
    double from_path_prob;

    // Parameters specific to Kronecker generator
    int mrg_seed_1;
    int mrg_seed_2;

    // Parameters specific to Power Law generator
    double concentration;
    unsigned int kseed;

    char *file_name;

  public:

    ConfigParser(char *config_file_name, int argc, char ** argv);
    int get_log_num_nodes();
    int get_edge_factor();
    int get_gen_type();
    int get_obf_type();
    int get_send_type();
    int get_parse_type();
    int get_receive_type();
    uint16_t get_to_gen_port();
    uint16_t get_to_resp_port();
    char* get_ip_address();
    int get_gen_rate();
    int get_output_interval();
    int get_per_packet();
    int get_result_buf_size();
    int get_mask_width();
    int get_mask_offset();
    int get_max_msg_len();
    int get_max_uname_size();
    int get_num_gen();
    int get_rand_seed();
    int get_min_len();
    int get_max_len();
    double get_gen_edge_prob();
    double get_gen_yes_prob();
    double get_from_path_prob();
    int get_mrg_seed_1();
    int get_mrg_seed_2();
    int get_concentration();
    int get_kseed();
    void dump();
    ~ConfigParser();
};

#endif //CONFIG_PARSER_H
