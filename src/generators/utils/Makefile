CXX=g++
LIB=lib
CPP=g++
INC=../../../include
OBJ=../../../obj
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS=-std=c++11


.PHONY: generator

generator: 
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/ListenThread.o ListenThread.cpp
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/ResultsBuffer.o ResultsBuffer.cpp
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/StreamGenerator.o StreamGenerator.cpp
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/GenSetupMethods.o GenSetupMethods.cpp

generator_prof: 
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/ListenThread_prof.o ListenThread.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/ResultsBuffer_prof.o ResultsBuffer.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/StreamGenerator_prof.o StreamGenerator.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/GenSetupMethods_prof.o GenSetupMethods.cpp
