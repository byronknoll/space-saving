OBJS = build/runner.o build/hasher.o build/hash-map.o build/space-saving.o
CC = g++
CFLAGS = -Wall -c -O3
LFLAGS = -Wall -O3

all : space-saving

space-saving : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o space-saving 

build/runner.o : src/runner.cpp src/hasher.h src/space-saving.h src/hash-map.h
	$(CC) $(CFLAGS) src/runner.cpp -o build/runner.o

build/hasher.o : src/hasher.h src/hasher.cpp
	$(CC) $(CFLAGS) src/hasher.cpp -o build/hasher.o

build/space-saving.o : src/space-saving.h src/space-saving.cpp src/hash-map.h
	$(CC) $(CFLAGS) src/space-saving.cpp -o build/space-saving.o

build/hash-map.o : src/hash-map.h src/hash-map.cpp
	$(CC) $(CFLAGS) src/hash-map.cpp -o build/hash-map.o

clean:
	rm -f -r build/* space-saving
