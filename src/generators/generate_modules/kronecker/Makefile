LIB=lib
SRC=.
CXX=g++
OBJ=../../../../obj
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS += $(INCLUDE) -Wall -std=gnu++0x -I ../../include
CFLAGS += $(objinc)
CFLAGS += -Drestrict=__restrict__ -DNDEBUG -ffast-math -DGRAPH_GENERATOR_SEQ

OBJECTS=graph_generator.o KronGen.o mrg_transitions.o splittable_mrg.o utils.o
OBJECTS_PROF=graph_generator_prof.o KronGen_prof.o mrg_transitions_prof.o splittable_mrg_prof.o utils_prof.o

kronecker: $(OBJECTS)

kronecker_prof: $(OBJECTS_PROF)

%.o: %.cpp
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/$@ $^

%.o: %.c
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/$@ $^

%_prof.o: %.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/$@ $^

%_prof.o: %.c
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/$@ $^
