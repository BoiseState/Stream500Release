CXX = g++
BIN=../bin
OBJ=../obj
LDFLAGS = -g -pg
OFLAGS = -O3
CFLAGS=-std=c++11

.PHONY: generator

socket:
	$(MAKE) -C sockets socket

generator:
	$(MAKE) -C generators generator 
#	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/Coverage.o Coverage.cpp
	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/ConfigParser.o ConfigParser.cpp
	$(CXX) $(CFLAGS) $(OFLAGS) -v -o $(BIN)/generator generator_engine.cpp  $(OBJ)/ConfigParser.o $(OBJ)/GenSetupMethods.o $(OBJ)/TCP_Client.o $(OBJ)/StreamGenerator.o $(OBJ)/ListenThread.o $(OBJ)/ResultsBuffer.o $(OBJ)/GeneratorObj.o $(OBJ)/ErdRenGen.o $(OBJ)/FromFileGen.o $(OBJ)/KronGen.o $(OBJ)/PowerGen.o $(OBJ)/splittable_mrg.o $(OBJ)/ObfuscateObj.o $(OBJ)/UDP_Server.o $(OBJ)/SendObj.o $(OBJ)/UdpSend.o $(OBJ)/utils.o -L${JAVA_HOME}/jre/lib/server/ $(OBJ)/TwitterObfuscator.o -ljvm -lpthread -lm
#	$(CXX) $(CFLAGS) $(OFLAGS) -o $(BIN)/generator generator_engine.cpp  $(OBJ)/ConfigParser.o $(OBJ)/GenSetupMethods.o $(OBJ)/TCP_Client.o $(OBJ)/StreamGenerator.o $(OBJ)/ListenThread.o $(OBJ)/ResultsBuffer.o $(OBJ)/GeneratorObj.o $(OBJ)/ErdRenGen.o $(OBJ)/FromFileGen.o $(OBJ)/KronGen.o $(OBJ)/PowerGen.o $(OBJ)/splittable_mrg.o $(OBJ)/ObfuscateObj.o $(OBJ)/UDP_Server.o $(OBJ)/SendObj.o $(OBJ)/UdpSend.o $(OBJ)/utils.o $(OBJ)/Coverage.o $(OBJ)/TwitterObfuscator.o -lpthread -lm

responder:
	$(MAKE) -C responders responder
#	$(CXX) $(CFLAGS) $(OFLAGS) -c -o $(OBJ)/Coverage.o Coverage.cpp
	$(CXX) $(CFLAGS) $(OFLAGS) -o $(BIN)/responder responder_engine.cpp  $(OBJ)/resp_eng_init.o $(OBJ)/TCP_Server.o $(OBJ)/UnionFindObj.o $(OBJ)/RespSetupMethods.o $(OBJ)/StreamProcessor.o $(OBJ)/ParseObj.o $(OBJ)/RecvObj.o $(OBJ)/UdpRecv.o $(OBJ)/TwitterParser.o -lm
#	$(CXX) $(CFLAGS) $(OFLAGS) -o $(BIN)/responder responder_engine.cpp  $(OBJ)/resp_eng_init.o $(OBJ)/TCP_Server.o $(OBJ)/UnionFindObj.o $(OBJ)/RespSetupMethods.o $(OBJ)/StreamProcessor.o $(OBJ)/ParseObj.o $(OBJ)/RecvObj.o $(OBJ)/UdpRecv.o $(OBJ)/Coverage.o $(OBJ)/TwitterParser.o -lm

socket_prof:
	$(MAKE) -C sockets socket_prof

generator_prof:
	$(MAKE) -C generators generator_prof
#	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/Coverage_prof.o Coverage.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/ConfigParser_prof.o ConfigParser.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $(BIN)/generator_prof generator_engine.cpp  $(OBJ)/ConfigParser_prof.o $(OBJ)/GenSetupMethods_prof.o $(OBJ)/TCP_Client_prof.o $(OBJ)/StreamGenerator_prof.o $(OBJ)/ListenThread_prof.o $(OBJ)/ResultsBuffer_prof.o $(OBJ)/GeneratorObj_prof.o $(OBJ)/ErdRenGen_prof.o $(OBJ)/FromFileGen_prof.o $(OBJ)/KronGen_prof.o $(OBJ)/PowerGen_prof.o $(OBJ)/splittable_mrg_prof.o $(OBJ)/ObfuscateObj_prof.o $(OBJ)/SendObj_prof.o $(OBJ)/UdpSend_prof.o $(OBJ)/utils_prof.o -lpthread -lm
#	$(CXX) $(CFLAGS) $(LDFLAGS) -o $(BIN)/generator_prof generator_engine.cpp  $(OBJ)/ConfigParser_prof.o $(OBJ)/GenSetupMethods_prof.o $(OBJ)/StreamGenerator_prof.o $(OBJ)/ListenThread_prof.o $(OBJ)/ResultsBuffer_prof.o $(OBJ)/GeneratorObj_prof.o $(OBJ)/ErdRenGen_prof.o $(OBJ)/FromFileGen_prof.o $(OBJ)/KronGen_prof.o $(OBJ)/PowerGen_prof.o $(OBJ)/splittable_mrg_prof.o $(OBJ)/ObfuscateObj_prof.o $(OBJ)/SendObj_prof.o $(OBJ)/UdpSend_prof.o $(OBJ)/utils_prof.o $(OBJ)/Coverage_prof.o $(OBJ)/TwitterObfuscator_prof.o -lpthread -lm

responder_prof:
	$(MAKE) -C responders responder_prof
#	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(OBJ)/Coverage_prof.o Coverage.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $(BIN)/responder_prof responder_engine.cpp  $(OBJ)/resp_eng_init_prof.o $(OBJ)/TCP_Server_prof.o $(OBJ)/UnionFindObj_prof.o $(OBJ)/RespSetupMethods_prof.o $(OBJ)/StreamProcessor_prof.o $(OBJ)/ParseObj_prof.o $(OBJ)/RecvObj_prof.o $(OBJ)/UdpRecv_prof.o $(OBJ)/TwitterParser_prof.o -lm
#	$(CXX) $(CFLAGS) $(LDFLAGS) -o $(BIN)/responder_prof responder_engine.cpp  $(OBJ)/resp_eng_init_prof.o $(OBJ)/TCP_Server_prof.o $(OBJ)/UnionFindObj_prof.o $(OBJ)/RespSetupMethods_prof.o $(OBJ)/StreamProcessor_prof.o $(OBJ)/ParseObj_prof.o $(OBJ)/RecvObj_prof.o $(OBJ)/UdpRecv_prof.o $(OBJ)/Coverage_prof.o $(OBJ)/TwitterParser_prof.o -lm

