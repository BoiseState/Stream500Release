BIN=bin
LIB=lib
SRC=src
LOGS=logs
CPP=g++
INC=include
OBJ=obj
TARGET=target/xeger
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS=-std=c++11
UNITTESTS=unitTests


.PHONY: generator

all: socket generator responder
all_prof: socket_prof generator_prof responder_prof

socket:
	mkdir -p $(BIN)
	mkdir -p $(OBJ)
	$(MAKE) -C $(SRC) socket

generator: 
	mkdir -p $(BIN)
	mkdir -p $(OBJ)
	mkdir -p $(TARGET)
	$(MAKE) -C $(SRC) generator 

responder:
	mkdir -p $(BIN)
	mkdir -p $(OBJ)
	$(MAKE) -C $(SRC) responder

socket_prof:
	mkdir -p $(BIN)
	mkdir -p $(OBJ)
	$(MAKE) -C $(SRC) socket_prof
generator_prof: 
	mkdir -p $(BIN)
	mkdir -p $(OBJ)
	$(MAKE) -C $(SRC) generator_prof

responder_prof:
	$(MAKE) -C $(SRC) responder_prof

tests:
	mkdir -p $(BIN)
	mkdir -p $(OBJ)
	mkdir -p $(LIB)
	$(MAKE) -C $(UNITTESTS) tests

coverage:
	$(MAKE) -C $(UNITTESTS) coverage

clean:
	rm -f $(BIN)/* $(OBJ)/* $(LIB)/* $(TARGET)/*
	rm -rf $(LOGS)
	$(MAKE) -C $(UNITTESTS) clean
