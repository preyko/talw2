YYAC = bison
LEX = flex

CPP = g++
CC = gcc




all:
	$(LEX) tokenizer.l
	$(YYAC) -d -v precompiler.y -o precompiler.cc

	$(CC) -lfl -c lex.yy.c -o tokenizer.o

	$(CPP) -ggdb -c precompiler.cc -o precompiler.o
	$(CPP) -ggdb precompiler.o tokenizer.o -o Precompiler
