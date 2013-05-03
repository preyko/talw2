YYAC = bison
LEX = flex

CPP = g++
CC = gcc

INCLUDDIR= .

BUILDDIR=build



all:
	mkdir -p $(BUILDDIR)

	$(LEX) -o $(BUILDDIR)/token_reader.c token_reader.l
	$(YYAC) -d precompiler.y -o $(BUILDDIR)/precompiler.cc
	
	tools/apts precompiler_namespaceo $(BUILDDIR)/precompiler.cc precompiler_namespacec
	tools/apts precompiler_namespaceo $(BUILDDIR)/precompiler.hh precompiler_namespacec
	tools/apts precompiler_namespaceo $(BUILDDIR)/token_reader.c precompiler_namespacec

	$(CPP) -lfl -c $(BUILDDIR)/token_reader.c -o $(BUILDDIR)/token_reader.o

	$(CPP) -ggdb -c $(BUILDDIR)/precompiler.cc -o $(BUILDDIR)/precompiler.o
	$(CPP) -ggdb $(BUILDDIR)/precompiler.o $(BUILDDIR)/token_reader.o -o Precompiler


tokenizer: tokenizer.l
	mkdir -p $(BUILDDIR)

	$(LEX) -o $(BUILDDIR)/tokenizer.c tokenizer.l
	$(CPP) -ggdb -DMAIN -I $(INCLUDDIR) $(BUILDDIR)/tokenizer.c -o Tokenizer


clear:
	rm -r $(BUILDDIR)
