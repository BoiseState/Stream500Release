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

#include "resp_eng_init.h"

resp_eng_init::resp_eng_init(char *config_file_name, int argc, char ** argv)
{
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
    else if (line.find("to_resp_port") != std::string::npos) {
      sin >> to_resp_port;
    }
    else if (line.find("to_gen_port") != std::string::npos) {
      sin >> to_gen_port;
    }
    else if (line.find("per_packet") != std::string::npos) {
      sin >> per_packet;
    }
    else if (line.find("num_gen") != std::string::npos) {
      sin >> num_gen;
    }
    else if (line.find("parse_type") != std::string::npos) {
      sin >> parse_type;
    }
    else if (line.find("receive_type") != std::string::npos) {
      sin >> receive_type;
    }
    else if (line.find("max_msg_len") != std::string::npos) {
      sin >> max_msg_len;
    }
    else if (line.find("max_uname_size") != std::string::npos) {
      sin >> max_uname_size;
    }
    sin.clear();
  }

  register int op;
  while ((op = getopt(argc, argv, "b:p:")) != EOF) {
    switch (op) {
      case 'b':
        per_packet = atoi(optarg);
        break;
      case 'p':
        num_gen = atoi(optarg);
        break;
    }
  }

  int flag = 0;
  if (per_packet <= 0) flag = 1;
  if (num_gen <= 0) flag = 1;
  if (flag) {
    fprintf(stderr, "ERROR: invalid command-line switch\n");
    exit(1);
  }
}

int resp_eng_init::get_log_num_nodes()
{
  return log_num_nodes;
}

uint16_t resp_eng_init::get_to_resp_port()
{
  return to_resp_port;
}

uint16_t resp_eng_init::get_to_gen_port()
{
  return to_gen_port;
}

int resp_eng_init::get_per_packet()
{
  return per_packet;
}

int resp_eng_init::get_num_gen()
{
  return num_gen;
}

int resp_eng_init::get_parse_type()
{
  return parse_type;
}

int resp_eng_init::get_receive_type()
{
  return receive_type;
}

int resp_eng_init::get_max_msg_len()
{
  return max_msg_len;
}

int resp_eng_init::get_max_uname_size()
{
  return max_uname_size;
}

