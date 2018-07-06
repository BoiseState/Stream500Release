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

#ifndef GEN_SETUP_METHODS_H
#define GEN_SETUP_METHODS_H

#include "../../ConfigParser.h"
#include "../data_types.h"
#include "../generate_modules/GeneratorObj.h"
#include "../generate_modules/erdosrenyi/ErdRenGen.h"
#include "../generate_modules/fromfile/FromFileGen.h"
#include "../generate_modules/kronecker/KronGen.h"
#include "../generate_modules/kronecker/utils.h"
#include "../generate_modules/powerlaw/PowerGen.h"
#include "../obfuscate_modules/ObfuscateObj.h"
#include "../obfuscate_modules/twitter_obfuscator/TwitterObfuscator.h"
#include "../send_modules/udp_send/UdpSend.h"

GeneratorObj* setup_generator(ConfigParser* cp);
ObfuscateObj* setup_obfuscator(ConfigParser* cp);
SendObj* setup_sender(ConfigParser* cp);

#endif //GEN_SETUP_METHODS_H
