all:
	flex rl.l
	bison -d rl.y -o rl.cc
	gcc -lfl -c lex.yy.c -o rl.o
	g++ rl.o rl.cc -o rltest 
