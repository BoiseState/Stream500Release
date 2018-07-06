CXX=g++
LIB=lib
CPP=g++
INC=../../../include
OBJ=../../../obj
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS=-std=c++11


.PHONY: generator

generator: SendObj.cpp
	$(MAKE) -C udp_send
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/SendObj.o SendObj.cpp

generator_prof: SendObj.cpp
	$(MAKE) -C udp_send udp_send_prof
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/SendObj_prof.o SendObj.cpp
