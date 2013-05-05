CC=g++

CCFLAGS=-ggdb
INCLUDE=-I ../RLPrecompiler
LIBNAME=rltype
LIBFILE=lib$(LIBNAME).a

all: comp_release compose
	
debug: comp_debug compose
	
comp_debug: RLType.cpp RLCommand.cpp
	$(CC) $(CCFLAGS) $(INCLUDE) -c RLType.cpp -o RLType.o
	$(CC) $(CCFLAGS) $(INCLUDE) -c RLInterpreter.cpp -o RLInterpreter.o
	$(CC) $(CCFLAGS) $(INCLUDE) -c RLCommand.cpp -o RLCommand.o

comp_release: RLType.cpp RLCommand.cpp
	$(CC) $(INCLUDE) -c RLType.cpp -o RLType.o
	$(CC) $(INCLUDE) -c RLInterpreter.cpp -o RLInterpreter.o
	$(CC) $(INCLUDE) -c RLCommand.cpp -o RLCommand.o

compose: RLCommand.o RLType.o RLInterpreter.o
	ar rc $(LIBFILE) RLCommand.o RLType.o RLInterpreter.o

clear:
	rm $(LIBFILE) RLCommand.o RLType.o RLInterpreter.o
