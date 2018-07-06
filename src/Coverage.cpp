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

#include "Coverage.h"

Coverage::Coverage(){
	lineCoverageLineNumber = 36;
	functionCoverageLineNumber = 45;
	threshold = 80.0;
}

int Coverage::verifyCoverage(){	
	std::regex rgx("<td class=\"headerCovTableEntryHi\">(.*) %<\/td>");
	std::smatch match;
	std::string fileName = "../unitTests/coverageOutput/index.html";
	std::ifstream f(fileName);
	std::string s;
	
	if(f.fail()){
		perror("Code coverage information not found");
		return 1;
	}
	for (int i = 1; i <= functionCoverageLineNumber; i++){
		std::getline(f, s);

		if(i==lineCoverageLineNumber){
			const std::string lineCoverageString = s;
			if (std::regex_search(lineCoverageString.begin(), lineCoverageString.end(), match, rgx)){			
				lineCoverage = match[1];
			} 	
		}

		if(i==functionCoverageLineNumber){
			const std::string functionCoverageLineNumber = s;
			if (std::regex_search(functionCoverageLineNumber.begin(), functionCoverageLineNumber.end(), match, rgx)){
				functionCoverage = match[1];
			}  		
		}
	}
	if(::atof(lineCoverage.c_str()) < threshold || ::atof(functionCoverage.c_str()) < threshold){
		std::cout<<"Line coverage or Function coverage is less than "<<threshold<<std::endl;
		return 1;
	}
	return 0;
}

Coverage::~Coverage(){

}


	