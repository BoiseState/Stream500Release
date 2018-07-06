CC = gcc
CXX = g++
BIN=../../bin
OBJ=../../obj
INCLUDE = -I./
LINK = -lm
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS=-std=c++11

.PHONY: generator

generator:
	$(MAKE) -C generate_modules 
	$(MAKE) -C obfuscate_modules 
	$(MAKE) -C send_modules
	$(MAKE) -C utils

generator_prof:
	$(MAKE) -C generate_modules generator_prof
	$(MAKE) -C obfuscate_modules generator_prof
	$(MAKE) -C send_modules generator_prof
	$(MAKE) -C utils generator_prof

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $<

