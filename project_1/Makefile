CXXFLAGS=-g -std=c++11 -Wall

MODULES=token.o lexer.o parser.o datalogprogram.o domain_collector.o seeker.o

all: main

pkg: main
	zip lab1.zip *.h *.cpp

main: $(MODULES) main.cpp
	g++ $(MODULES) main.cpp -o main -Wall -std=c++11 -g

test: inputs/* *.cpp *.h
	./test.sh

mem:
	valgrind --leak-check=full ./main proj_2_tests/in21.txt -

comp:
	pmccabe *.cpp *.h
