CFLAGS =  -g  -std=c++11
SHELL := /bin/bash

all: Cache.o MemAccess.h main.o
	g++ $(CFLAGS) Cache.o main.o -o cache-sim

Cache.o: Cache.cpp Cache.h
	g++ $(CFLAGS) -c Cache.cpp

main.o: main.cpp
	g++ $(CFLAGS) -c main.cpp


clean:
	rm -rf *.o cache-sim
