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

#include"TwitterObfuscator.h"
#include<iostream>
#include <string.h>
#include "jni.h"

/*************************************************************************************************************
 * Goal:
 * The goal of this class is to generate a twitter style message, where obfuscation of vertices is done using 
 * regular expression or other mechanisms to replicate the real world communication between two persons.
 * Two persons means two vertices here.

 * Explanation:
 * Xeger(a java library) has been implemented using JNI to generate string from given regular expression.
 * Two vertices has been obfuscated (Edge/query having vertices (v0,v1) is converted to a obfuscated 
 * message junk_message1 obfuscated_v0 junk_message2 obfuscated_v1) 
**************************************************************************************************************/

JavaVMOption options[1];
JNIEnv *env;
JavaVM *jvm;
JavaVMInitArgs vm_args;
int numBytes;
jclass cls;

//Constructor
TwitterObfuscator::TwitterObfuscator(){
  options[0].optionString = const_cast<char *>("-Djava.class.path=./target:./jar/xeger-1.0-SNAPSHOT.jar:./jar/automaton.jar");
  memset(&vm_args, 0, sizeof(vm_args));
  vm_args.version = JNI_VERSION_1_8;
  vm_args.nOptions = 1;
  vm_args.options = options;

  long status = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
  if( status != JNI_OK ) {
    perror("JNI creation Failed : ");
  }
  cls = env->FindClass("xeger/RegexToString");
  if(cls == 0){
  	perror("JNI FindClass error :");
  }
}


/******************************************************************************
 * Parameters:
 * e - edge pointer containing values of an edge
 * max_buf_size - maximum size of message for an obfuscated edge
 * msg_buf - buffer for storing the obfuscated edge
 * Goal:
 * Obfuscate an edge using some junk text. Junk text supposed to be generated using regular expression to 
 * string generator(For example : xeger). Then the Responder has to do regular expression processing to 
 * parse vertices associated with a given edge.
*******************************************************************************/
int32_t TwitterObfuscator::obfuscate_edge(edge *e, int max_buf_size, void *msg_buf)
{
  jmethodID mid = env->GetStaticMethodID(cls, "obfuscateVertices", "(II)Ljava/lang/String;");
  if( mid != 0 ) {
    jstring temp = (jstring)  env->CallObjectMethod(cls, mid,e->v0,e->v1);     
    const char *regexGeneratedString = env->GetStringUTFChars(temp, 0);
    numBytes = snprintf(((char*)msg_buf), max_buf_size, regexGeneratedString);
  } 
  else	perror("JNI GetStaticMethodID error : ");
  return numBytes;
}


/******************************************************************************
 * Parameters:
 * q - query pointer containing values of a query
 * max_buf_size - maximum size of message for an obfuscated query
 * msg_buf - buffer for storing the obfuscated query
 * Goal:
 * Obfuscate the vertex ID's of a query the same way as edges are obfuscated
 * so they appear the same to the Responder. Query ID's should not be obfuscated
 * so they can be matched when checking results. 
*******************************************************************************/
int32_t TwitterObfuscator::obfuscate_query(query *q, int max_buf_size, void *msg_buf)
{
  jmethodID mid = env->GetStaticMethodID(cls, "obfuscateVertices", "(II)Ljava/lang/String;");
  if( mid != 0 ) {
    jstring temp = (jstring)  env->CallObjectMethod(cls, mid, q->v0, q->v1);     
    const char *regexGeneratedString = env->GetStringUTFChars(temp, 0);
    numBytes = snprintf(((char*)msg_buf), max_buf_size, regexGeneratedString);
  } 
  else perror("JNI GetStaticMethodID error : ");
  return numBytes;
}

//destructor
TwitterObfuscator::~TwitterObfuscator(){
	jvm->DestroyJavaVM();
}
