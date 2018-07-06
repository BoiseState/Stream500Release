CXX=g++
LIB=lib
CPP=g++
INC=../../../include
OBJ=../../../obj
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS=-std=c++11


.PHONY: responder

responder: ParseObj.cpp
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/ParseObj.o ParseObj.cpp
	$(MAKE) -C twitter_parser

responder_prof: ParseObj.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/ParseObj_prof.o ParseObj.cpp
#	$(MAKE) -C twitter_parser twitter_parser_prof
