all: cacheSim

# Environment for C
CC = gcc
CFLAGS = -std=c99 -Wall

# Environment for C++
CXX = g++
CXXFLAGS = -std=c++11 -Wall

cacheSim: cacheSim.cpp
	$(CXX) -o $@ $^ -lm

MemoryManager.o: MemoryManager.cpp MemoryManager.h
	$(CXX) -c $(CXXFLAGS)  -o $@ $^ -lm

Cache.o: Cache.cpp Cache.h
	$(CXX) -c $(CXXFLAGS)  -o $@ $^ -lm


.PHONY: clean
clean:
	rm -f *.o
	rm -f cacheSim
