CC = gcc
CXX = g++
BIN=../../bin
OBJ=../../obj
INCLUDE = -I./
LINK = -lm
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS=-std=c++11

.PHONY: responder

responder:
	$(MAKE) -C parse_modules 
	$(MAKE) -C receive_modules
	$(MAKE) -C utils
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/resp_eng_init.o resp_eng_init.cpp

responder_prof:
	$(MAKE) -C parse_modules responder_prof
	$(MAKE) -C receive_modules responder_prof
	$(MAKE) -C utils responder_prof
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/resp_eng_init_prof.o resp_eng_init.cpp

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $<

