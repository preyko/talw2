CC=g++

CCFLAGS=-ggdb

LIBNAME=rltype
LIBFILE=lib$(LIBNAME).a

all: comp_release compose
	
debug: comp_debug compose
	

comp_debug: RLType.* RLCommand.*
	$(CC) $(CCFLAGS) -c RLType.cpp -o RLType.o
	$(CC) $(CCFLAGS) -c RLCommand.cpp -o RLCommand.o

comp_release: RLType.* RLCommand.*
	$(CC) -c RLType.cpp -o RLType.o
	$(CC) -c RLCommand.cpp -o RLCommand.o

compose: RLCommand.o RLType.o
	ar rc $(LIBFILE) RLCommand.o RLType.o

clear:
	rm $(LIBFILE) RLCommand.o RLType.o
