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

#include <iostream>
#include "ConfigParser.h"
#include "./generators/utils/GenSetupMethods.h"
#include "./generators/utils/StreamGenerator.h"
#include "Coverage.h"


int main(int argc, char **argv)
{
  // Coverage *cvrg = new Coverage();
  // if(cvrg->verifyCoverage()==1){
  //   std::cout<<"Build Failure"<<std::endl;
  //   return 0;
  // }
//  int which_gen;

//Consider only allowing config file first and obtaining from argv...
  ConfigParser *cp;
  StreamGenerator *sg;

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
  cp = new ConfigParser(config_file_name, argc, argv);

  // Setup objects for generating, obfuscating, and sending
  GeneratorObj* generator = setup_generator(cp);
  ObfuscateObj* obfuscator = setup_obfuscator(cp); 
  SendObj* sender = setup_sender(cp);

  // Begin generating
  sg = new StreamGenerator(cp);
  double time_start = sg->my_clock();
  sg->generate_stream(generator, obfuscator, sender, time_start);
  double time_stop = sg->my_clock();
  sg->end_stream(sender);
  cp->dump();
  sg->print_results(time_start, time_stop);
 
  delete(generator);
  delete(obfuscator);
  delete(sender);
  delete(sg);
  delete(cp);
  // delete(cvrg);
  return 0;
}

