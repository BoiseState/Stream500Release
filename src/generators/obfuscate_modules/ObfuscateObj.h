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

#ifndef OBFUSCATEOBJ_H
#define OBFUSCATEOBJ_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../data_types.h"

class ObfuscateObj
{
  public:
    ObfuscateObj();
    virtual int32_t obfuscate_edge(edge *e, int max_buf_size, void *msg_buf);
    virtual int32_t obfuscate_query(query *q, int max_buf_size, void *msg_buf);
    virtual ~ObfuscateObj();
};

#endif //OBFUSCATEOBJ_H
