CC=g++

CCPARAMS=-DRLINTERP_DEBUG -ggdb

LIBNAME=rltypes
LIBFILE=lib$(LIBNAME).a

all: comp_release compose
	
debug: comp_debug compose
	

comp_debug: RLType.*
	$(CC) $(CCPARAMS) -c RLType.cpp -o $(LIBNAME).o

comp_release: RLType.*
	$(CC) -c RLType.cpp -o $(LIBNAME).o

compose: $(LIBNAME).o
	ar rc $(LIBFILE) $(LIBNAME).o

clear:
	rm $(LIBFILE) $(LIBNAME).o
