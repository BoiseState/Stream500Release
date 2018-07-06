CXX=g++
LIB=lib
CPP=g++
INC=../../../../include
OBJ=../../../../obj
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS=-std=c++11


.PHONY: powerlaw 


powerlaw: PowerGen.o

powerlaw_prof: PowerGen_prof.o

%.o: %.cpp
	$(CPP) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/$@ $^

%_prof.o: %.cpp
	$(CPP) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/$@ $^
