CXX=g++
LIB=lib
CPP=g++
INC=../../../../include
OBJ=../../../../obj
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS=-std=c++11
ROOTDIR=../../../..


.PHONY: generator


twitter_obfuscator: TwitterObfuscator.o $(ROOTDIR)/target/xeger/RegexToString.class

twitter_obfuscator_prof: TwitterObfuscator_prof.o

%.o: %.cpp
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/$@ $^ -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux/

%_prof.o: %.cpp
	$(CPP) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/$@ $^

$(ROOTDIR)/target/xeger/RegexToString.class: $(ROOTDIR)/java/xeger/RegexToString.java
	$(JAVA_HOME)/bin/javac -cp $(ROOTDIR)/jar/xeger-1.0-SNAPSHOT.jar -d $(ROOTDIR)/target $<
