CXX=g++
LIB=lib
CPP=g++
INC=../../../include
OBJ=../../../obj
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS=-std=c++11


.PHONY: responder

responder: RecvObj.cpp
	$(MAKE) -C udp_recv
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/RecvObj.o RecvObj.cpp

responder_prof: RecvObj.cpp
	$(MAKE) -C udp_recv udp_recv_prof
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/RecvObj_prof.o RecvObj.cpp
