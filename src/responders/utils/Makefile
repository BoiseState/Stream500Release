CXX=g++
LIB=lib
CPP=g++
INC=../../../include
OBJ=../../../obj
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS=-std=c++11


.PHONY: responder

responder: 
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/UnionFindObj.o UnionFindObj.cpp
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/RespSetupMethods.o RespSetupMethods.cpp
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/StreamProcessor.o StreamProcessor.cpp

responder_prof: 
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/UnionFindObj_prof.o UnionFindObj.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/RespSetupMethods_prof.o RespSetupMethods.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/StreamProcessor_prof.o StreamProcessor.cpp
