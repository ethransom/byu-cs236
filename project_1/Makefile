CFLAGS=-g -std=c++11 -Wall

all: main

pkg: main
	zip project1.zip *.h *.cpp

main: *.cpp *.h
	g++ *.cpp -o main -std=c++14 -g

test: inputs/* *.cpp *.h
	./test.sh

mem:
	valgrind --leak-check=full ./main inputs/in12.txt

comp:
	pmccabe *.cpp *.h
