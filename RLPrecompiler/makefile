YYAC = bison
LEX = flex

CPP = g++
CC = gcc

LIBNAME=rlprecomp
LIBFILE=lib$(LIBNAME).a

INCLUDDIR= . -I ../rlinterpreter
LIBINC = -L ../rlinterpreter -l rltype

BUILDDIR=build


all: process_precompiller
	$(CPP) -I $(INCLUDDIR) -lfl -c $(BUILDDIR)/token_reader.c -o $(BUILDDIR)/token_reader.o
	$(CPP) -I $(INCLUDDIR) -ggdb -DMAIN -c $(BUILDDIR)/precompiler.cc -o $(BUILDDIR)/precompiler.o

	$(CPP) -I $(INCLUDDIR) -ggdb $(BUILDDIR)/precompiler.o $(BUILDDIR)/token_reader.o $(LIBINC) -o Precompiler

process_precompiller: token_reader.l precompiler.y
	mkdir -p $(BUILDDIR)

	$(LEX) -o $(BUILDDIR)/token_reader.c token_reader.l
	$(YYAC) -d -v precompiler.y -o $(BUILDDIR)/precompiler.cc
	
	tools/apts precompiler_head $(BUILDDIR)/precompiler.cc precompiler_tail
	tools/apts token_reader_head $(BUILDDIR)/precompiler.hh token_reader_tail


process_tokenizer: tokenizer.l
	mkdir -p $(BUILDDIR)

	$(LEX) -o $(BUILDDIR)/tokenizer.c tokenizer.l

tokenizer: process_tokenizer 
	$(CPP) -I $(INCLUDDIR) -ggdb -DMAIN $(BUILDDIR)/tokenizer.c -o Tokenizer

lib: process_precompiller process_tokenizer
	$(CPP) -I $(INCLUDDIR) -ggdb -lfl -c $(BUILDDIR)/token_reader.c -o $(BUILDDIR)/token_reader.o
	$(CPP) -I $(INCLUDDIR) -ggdb -c $(BUILDDIR)/precompiler.cc -o $(BUILDDIR)/precompiler.o
	$(CPP) -I $(INCLUDDIR) -ggdb -c $(BUILDDIR)/tokenizer.c -o $(BUILDDIR)/tokenizer.o

	ar rc $(BUILDDIR)/$(LIBFILE) $(BUILDDIR)/token_reader.o $(BUILDDIR)/precompiler.o $(BUILDDIR)/tokenizer.o

clear:
	rm -r $(BUILDDIR)
