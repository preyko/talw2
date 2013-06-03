INCLUDES=-I../rlinterpreter -I../RLPrecompiler

LIBS=-L../rlinterpreter -L../RLPrecompiler/build -lrltype -lrlprecomp

all: main.cpp
	g++ -ggdb $(INCLUDES) -c main.cpp -o build/main.o
	g++ build/main.o $(LIBS) -o build/rlprecompiler
