CXX=g++
LIB=lib
CPP=g++
INC=../../../../include
OBJ=../../../../obj
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS=-std=c++11


.PHONY: generator


udp_send: UdpSend.o

udp_send_prof: UdpSend_prof.o

%.o: %.cpp
	$(CPP) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/$@ $^

%_prof.o: %.cpp
	$(CPP) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/$@ $^
