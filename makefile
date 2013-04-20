all:
	flex rl.l
	bison -d -v rl.y -o rl.cc
	gcc -ggdb -lfl -c lex.yy.c -o rl.o
	g++ -ggdb -c rl.cc -o rl.cc.o
	g++ -ggdb rl.o rl.cc.o -L rltype -l rltype -o rltest 
