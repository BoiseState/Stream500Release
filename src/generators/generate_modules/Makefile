CXX=g++
LIB=lib
CPP=g++
INC=../../../include
OBJ=../../../obj
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS=-std=c++11


.PHONY: generator

generator: GeneratorObj.cpp
	$(MAKE) -C kronecker 
	$(MAKE) -C erdosrenyi
	$(MAKE) -C fromfile
	$(MAKE) -C powerlaw
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/GeneratorObj.o GeneratorObj.cpp

generator_prof: GeneratorObj.cpp
	$(MAKE) -C kronecker kronecker_prof
	$(MAKE) -C erdosrenyi erdosrenyi_prof
	$(MAKE) -C fromfile fromfile_prof
	$(MAKE) -C powerlaw powerlaw_prof
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/GeneratorObj_prof.o GeneratorObj.cpp
