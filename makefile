OBJS	= MemoryManager.o Set.o CacheLine.o cacheSim.o Cache.o
SOURCE	= MemoryManager.cpp Set.cpp CacheLine.cpp cacheSim.cpp Cache.cpp
HEADER	= Logger.h MemoryManager.h Set.h CacheLine.h Cache.h
OUT	= cacheSim
CC	 = g++
FLAGS	 = -g3 -c -Wall
LFLAGS	 = -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

MemoryManager.o: MemoryManager.cpp
	$(CC) $(FLAGS) MemoryManager.cpp -std=c++11

Set.o: Set.cpp
	$(CC) $(FLAGS) Set.cpp -std=c++11

CacheLine.o: CacheLine.cpp
	$(CC) $(FLAGS) CacheLine.cpp -std=c++11

cacheSim.o: cacheSim.cpp
	$(CC) $(FLAGS) cacheSim.cpp -std=c++11

Cache.o: Cache.cpp
	$(CC) $(FLAGS) Cache.cpp -std=c++11


clean:
	rm -f $(OBJS) $(OUT)

debug: $(OUT)
	valgrind $(OUT)

valgrind: $(OUT)
	valgrind $(OUT)

valgrind_leakcheck: $(OUT)
	valgrind --leak-check=full $(OUT)

valgrind_extreme: $(OUT)
	valgrind --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --vgdb=yes $(OUT)