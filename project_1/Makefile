CFLAGS=-g -std=c++11 -Wall

all: main

pkg: main
	zip project1.zip *.h *.cpp

main: main.cpp scanner.o filestream.o
	g++ -o main main.cpp scanner.o filestream.o $(CFLAGS)
