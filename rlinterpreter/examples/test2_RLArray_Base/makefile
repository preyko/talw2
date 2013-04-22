all: main.cpp
	g++ -fpermissive -ggdb -c main.cpp -I ../../ -o main.o
	g++ -fpermissive main.o -L ../../ -l rltype -o test2

withoutlib: ../../RLType.cpp main.cpp	
	g++ -fpermissive -ggdb -c ../../RLType.cpp -I ../../ -o RLType.o
	g++ -fpermissive -ggdb -c main.cpp -I ../../ -o main.o
	g++ RLType.o main.o -o test2
