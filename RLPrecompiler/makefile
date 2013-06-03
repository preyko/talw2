YYAC = bison
LEX = flex

CPP = g++
CC = gcc

CFLAGS=-fpermissive

LIBNAME=rlprecomp
LIBFILE=lib$(LIBNAME).a

BUILDDIR=build

INCLUDDIR= -I . -I ../rlinterpreter -I $(BUILDDIR)/
LIBINC = -L ../rlinterpreter -l rltype

all: $(BUILDDIR)/precompiler.o $(BUILDDIR)/token_reader.o $(BUILDDIR)/tokenizer.o
	ar rc $(BUILDDIR)/$(LIBFILE) $(BUILDDIR)/token_reader.o $(BUILDDIR)/precompiler.o $(BUILDDIR)/tokenizer.o

$(BUILDDIR)/precompiler.cc: precompiler.y precompiler_head precompiler_tail precompilerhh_head precompilerhh_tail
	mkdir -p $(BUILDDIR)
	$(YYAC) -d $(DEBUG_YYAC) precompiler.y -o $(BUILDDIR)/precompiler.cc
	tools/apts precompiler_head $(BUILDDIR)/precompiler.cc precompiler_tail
	tools/apts precompilerhh_head $(BUILDDIR)/precompiler.hh precompilerhh_tail

$(BUILDDIR)/token_reader.c: token_reader.l
	$(LEX) -o $(BUILDDIR)/token_reader.c token_reader.l

$(BUILDDIR)/tokenizer.c: tokenizer.l
	$(LEX) -o $(BUILDDIR)/tokenizer.c tokenizer.l


$(BUILDDIR)/precompiler.o: $(BUILDDIR)/precompiler.cc
	$(CPP) $(CFLAGS) $(INCLUDDIR) $(DEBUG) -c $(BUILDDIR)/precompiler.cc -o $(BUILDDIR)/precompiler.o

$(BUILDDIR)/token_reader.o: $(BUILDDIR)/token_reader.c
	$(CPP) $(CFLAGS) $(INCLUDDIR) $(DEBUG) -c $(BUILDDIR)/token_reader.c -o $(BUILDDIR)/token_reader.o

$(BUILDDIR)/tokenizer.o: $(BUILDDIR)/tokenizer.c
	$(CPP) $(CFLAGS) $(INCLUDDIR) $(DEBUG) -c $(BUILDDIR)/tokenizer.c -o $(BUILDDIR)/tokenizer.o


clean:
	rm -r $(BUILDDIR)
