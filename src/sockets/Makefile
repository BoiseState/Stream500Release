CC = gcc
CXX = g++
BIN=../../bin
OBJ=../../obj
INCLUDE = -I./
LINK = -lm
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS=-std=c++11

.PHONY: socket

socket:
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/UDP_Server.o UDP_Server.cpp
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/TCP_Server.o TCP_Server.cpp
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/TCP_Client.o TCP_Client.cpp

socket_prof:
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/UDP_Server_prof.o UDP_Server.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/TCP_Server_prof.o TCP_Server.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/TCP_Client_prof.o TCP_Client.cpp

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $<

