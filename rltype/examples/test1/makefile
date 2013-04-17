all: main.cpp
	g++ -ggdb -c main.cpp -I ../../ -o main.o
	g++ main.o -L ../../ -l rltypes -o test1

withoutlib: ../../RLType.cpp main.cpp	
	g++ -ggdb -c ../../RLType.cpp -I ../../ -o RLType.o
	g++ -ggdb -c main.cpp -I ../../ -o main.o
	g++ RLType.o main.o -o test1
