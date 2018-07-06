CXX=g++
LIB=lib
CPP=g++
INC=../../../include
OBJ=../../../obj
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS=-std=c++11


.PHONY: generator

generator: ObfuscateObj.cpp
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/ObfuscateObj.o ObfuscateObj.cpp
	$(MAKE) -C twitter_obfuscator

generator_prof: ObfuscateObj.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/ObfuscateObj_prof.o ObfuscateObj.cpp
	# $(MAKE) -C twitter_obfuscator twitter_obfuscator_prof
