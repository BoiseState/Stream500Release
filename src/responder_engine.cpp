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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

//#include "ConfigParser.h"
#include "./responders/resp_eng_init.h"
#include "./responders/utils/RespSetupMethods.h"
#include "./responders/utils/StreamProcessor.h"
#include "Coverage.h"
#include "./sockets/TCP_Server.h"


int main(int argc, char **argv)
{
  // Coverage *cvrg = new Coverage();
  // if(cvrg->verifyCoverage()==1){
  //   std::cout<<"Build Failure"<<std::endl;
  //   return 0;
  // }

  StreamProcessor *sp;
  char *config_file_name;
  int op;
  while ((op = getopt(argc, argv, "c:")) != -1)
  {
    switch(op)
    {
      case 'c':
        config_file_name = optarg;
        break;
    }
  }
//  ConfigParser cp(config_file_name, argc, argv);
  resp_eng_init* ri = new resp_eng_init(config_file_name, argc, argv);

  // Setup objects for receiving and parsing
  RecvObj *rcvr = setup_receiver(ri);
  ParseObj *parser = setup_parser(ri);
  sp = new StreamProcessor(ri);
  TCP_Server* sender = new TCP_Server(ri->get_to_gen_port());

  //Begin processing
  sp->process_stream(ri, rcvr, parser, sender);
  sp->finish_up(sender);
  sp->print_results();

  delete(rcvr);
  delete(parser);
  delete(sp);
  delete(ri);
  delete(sender);
  // delete(cvrg);
  return 0;
}
