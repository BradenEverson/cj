CC=clang
CFLAGS=-Wall -g -c

all: hash

hash: cj.o hash.o
	$(CC) -o hash cj.o hash.o

hash.o: hash.c cj.h
	$(CC) $(CFLAGS) -o hash.o hash.c

cj.o: cj.c cj.h
	$(CC) $(CFLAGS) -o cj.o cj.c

clean:
	rm -f hash *.o
