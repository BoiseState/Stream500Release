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

#include "ConfigParser.h"
#include <iostream>

/******************************************************************************
 * Parameters:
 *
*******************************************************************************/
ConfigParser::ConfigParser(char *config_file_name, int argc, char ** argv)
{
  ip_address = (char*)malloc(15);
  std::ifstream fin(config_file_name);
  std::string line;
  std::istringstream sin;
  if (!fin.is_open())
  {
    perror("error while opening file");
    exit(1);
  }
  while (std::getline(fin, line))
  {
    sin.str(line.substr(line.find(":")+1));
    if (line.find("log_num_nodes") != std::string::npos) {
      sin >> log_num_nodes;
    }
    else if (line.find("edge_factor") != std::string::npos) {
      sin >> edge_factor;
    }
    else if (line.find("gen_type") != std::string::npos) {
      sin >> gen_type;
    }
    else if (line.find("obf_type") != std::string::npos) {
      sin >> obf_type;
    }
    else if (line.find("send_type") != std::string::npos) {
      sin >> send_type;
    }
    else if (line.find("parse_type") != std::string::npos) {
      sin >> parse_type;
    }
    else if (line.find("receive_type") != std::string::npos) {
      sin >> receive_type;
    }
    else if (line.find("to_resp_port") != std::string::npos) {
      sin >> to_resp_port;
    }
    else if (line.find("to_gen_port") != std::string::npos) {
      sin >> to_gen_port;
    }
    else if (line.find("ip_address") != std::string::npos) {
      //Need to trim white space to make sure socket gets correct ip_address
      int i;
      int trimmed_index = 0;
      for (i = 0; i < sin.str().length(); i++)
      {
        char c = sin.str().c_str()[i];
        if (c != ' ' && c != '\t')
        {
          ip_address[trimmed_index] = c;
          trimmed_index++;
        }
      }
      ip_address[trimmed_index] = '\0';
    }
    else if (line.find("gen_rate") != std::string::npos) {
      sin >> gen_rate;
    }
    else if (line.find("output_interval") != std::string::npos) {
      sin >> output_interval;
    }
    else if (line.find("per_packet") != std::string::npos) {
      sin >> per_packet;
    }
    else if (line.find("result_buf_size") != std::string::npos) {
      sin >> result_buf_size;
    }
    else if (line.find("mask_width") != std::string::npos) {
      sin >> mask_width;
    }
    else if (line.find("mask_offset") != std::string::npos) {
      sin >> mask_offset;
    }
    else if (line.find("max_msg_len") != std::string::npos) {
      sin >> max_msg_len;
    }
    else if (line.find("max_uname_size") != std::string::npos) {
      sin >> max_uname_size;
    }
    else if (line.find("num_gen") != std::string::npos) {
      sin >> num_gen;
    }
    else if (line.find("rand_seed") != std::string::npos) {
      sin >> rand_seed;
    }
    else if (line.find("min_length") != std::string::npos) {
      sin >> min_length;
    }
    else if (line.find("max_length") != std::string::npos) {
      sin >> max_length;
    }
    else if (line.find("gen_edge_prob") != std::string::npos) {
      sin >> gen_edge_prob;
    }
    else if (line.find("gen_yes_prob") != std::string::npos) {
      sin >> gen_yes_prob;
    }
    else if (line.find("from_path_prob") != std::string::npos) {
      sin >> from_path_prob;
    }
    else if (line.find("mrg_seed_1") != std::string::npos) {
      sin >> mrg_seed_1;
    }
    else if (line.find("mrg_seed_2") != std::string::npos) {
      sin >> mrg_seed_2;
    }
    else if (line.find("concentration") != std::string::npos) {
      sin >> concentration;
    }
    else if (line.find("kseed") != std::string::npos) {
      sin >> kseed;
    }
    sin.clear();
  }

  register int op;
  while ((op = getopt(argc,argv,"s:b:p:f:")) != EOF) {
    switch (op) {
      case 's':
        rand_seed = atoi(optarg);
        break;
      case 'b':
        per_packet = atoi(optarg);
        break;
      case 'p':
        num_gen = atoi(optarg);
        break;
      case 'f':
        file_name = optarg;
        break;
    }
  }

  int flag = 0;
  if (per_packet <= 0) flag = 1;
  if (flag) {
    fprintf(stderr, "ERROR: invalid command-line switch\n");
    exit(1);
  }

  //Check mask fits for number of nodes...
}

int ConfigParser::get_log_num_nodes()
{
  return log_num_nodes;
}

int ConfigParser::get_edge_factor()
{
  return edge_factor;
}

int ConfigParser::get_gen_type()
{
  return gen_type;
}
    
int ConfigParser::get_obf_type()
{
  return obf_type;
}
    
int ConfigParser::get_send_type()
{
  return send_type;
}
    
int ConfigParser::get_parse_type()
{
  return parse_type;
}
    
int ConfigParser::get_receive_type()
{
  return receive_type;
}
    
uint16_t ConfigParser::get_to_gen_port()
{
  return to_gen_port;
}
    
uint16_t ConfigParser::get_to_resp_port()
{
  return to_resp_port;
}

char* ConfigParser::get_ip_address()
{
  char* copy_of_ip = (char*)malloc(15);
  char* temp_copy = copy_of_ip;
  char* temp_orig = ip_address;
  while (*ip_address != '\0')
  {
    *copy_of_ip = *ip_address;
    copy_of_ip++;
    ip_address++;
  }
  *copy_of_ip = '\0';
  copy_of_ip = temp_copy;
  ip_address = temp_orig;
  return copy_of_ip;
}

int ConfigParser::get_gen_rate()
{
  return gen_rate;
}
    
int ConfigParser::get_output_interval()
{
  return output_interval;
}
    
int ConfigParser::get_per_packet()
{
  return per_packet;
}
    
int ConfigParser::get_result_buf_size()
{
  return result_buf_size;
}
    
int ConfigParser::get_mask_width()
{
  return mask_width;
}
    
int ConfigParser::get_mask_offset()
{
  return mask_offset;
}
    
int ConfigParser::get_max_msg_len()
{
  return max_msg_len;
}

int ConfigParser::get_max_uname_size()
{
  return max_uname_size;
}
    
int ConfigParser::get_num_gen()
{
  return num_gen;
}
    
int ConfigParser::get_rand_seed()
{
  return rand_seed;
}
    
int ConfigParser::get_min_len()
{
  return min_length;
}
    
int ConfigParser::get_max_len()
{
  return max_length;
}
    
double ConfigParser::get_gen_edge_prob()
{
  return gen_edge_prob;
}
    
double ConfigParser::get_gen_yes_prob()
{
  return gen_yes_prob;
}
    
double ConfigParser::get_from_path_prob()
{
  return from_path_prob;
}
    
int ConfigParser::get_mrg_seed_1()
{
  return mrg_seed_1;
}

int ConfigParser::get_mrg_seed_2()
{
  return mrg_seed_2;
}

int ConfigParser::get_concentration()
{
  return concentration;
}
    
int ConfigParser::get_kseed()
{
  return kseed;
}

void ConfigParser::dump()
{
  printf("==================\n");
  printf("#CONFIG SETTINGS\n");
  printf("==================\n\n");
  if (log_num_nodes >= 0)
  {
    printf("log_num_nodes : %d\n", log_num_nodes);
  }
  if (edge_factor >= 0)
  {
    printf("edge_factor : %d\n", edge_factor);
  }
  if (gen_type >= 0)
  {
    printf("gen_type : %d\n", gen_type);
  }
  if (obf_type >= 0)
  {
    printf("obf_type : %d\n", obf_type);
  }
  if (send_type >= 0)
  {
    printf("send_type : %d\n", send_type);
  }
  if (parse_type >= 0)
  {
    printf("parse_type : %d\n", parse_type);
  }
  if (receive_type >= 0)
  {
    printf("receive_type : %d\n", receive_type);
  }
  if (to_resp_port >= 0)
  {
    printf("to_resp_port : %d\n", to_resp_port);
  }
  if (to_gen_port >= 0)
  {
    printf("to_gen_port : %d\n", to_gen_port);
  }
  if(ip_address)
  {
    printf("ip_address : %s\n", ip_address);
  }
  if (gen_rate != 0)
  {
    printf("gen_rate : %d\n", gen_rate);
  }
  if (output_interval >= 0)
  {
    printf("output_interval : %d\n", output_interval);
  }
  if (per_packet >= 0)
  {
    printf("per_packet : %d\n", per_packet);
  }
  if (result_buf_size >= 0)
  {
    printf("result_buf_size : %d\n", result_buf_size);
  }
  if (mask_width >= 0)
  {
    printf("mask_width : %d\n", mask_width);
  }
  if (mask_offset >= 0)
  {
    printf("mask_offset : %d\n", mask_offset);
  }
  if (max_msg_len >= 0)
  {
    printf("max_msg_len : %d\n", max_msg_len);
  }
  if (max_uname_size >= 0)
  {
    printf("max_uname_size : %d\n", max_uname_size);
  }
  if (num_gen >= 0)
  {
    printf("num_gen : %d\n", num_gen);
  }
  printf("rand_seed : %d\n", rand_seed);
  if (min_length >= 0)
  {
    printf("min_length : %d\n", min_length);
  }
  if (max_length >= 0)
  {
    printf("max_length : %d\n", max_length);
  }
  if (gen_edge_prob >= 0)
  {
    printf("gen_edge_prob : %g\n", gen_edge_prob);
  }
  if (gen_yes_prob >= 0)
  {
    printf("gen_yes_prob : %g\n", gen_yes_prob);
  }
  if (from_path_prob >= 0)
  {
    printf("from_path_prob : %g\n", from_path_prob);
  }
  if (mrg_seed_1 >= 0)
  {
    printf("mrg_seed_1 : %d\n", mrg_seed_1);
  }
  if (mrg_seed_2 >= 0)
  {
    printf("mrg_seed_2 : %d\n", mrg_seed_2);
  }
  if (concentration >= 0)
  {
    printf("concentration : %g\n", concentration);
  }
  if (kseed >= 0)
  {
    printf("kseed : %d\n", kseed);
  }
  printf("\n\n");
}

ConfigParser::~ConfigParser()
{
  free(ip_address);
}
