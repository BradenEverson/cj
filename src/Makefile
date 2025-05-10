CC=clang
CFLAGS=-Wall -g -c

all: hash tok_stream

hash: cj.o hash.o
	$(CC) -o hash cj.o hash.o

tok_stream: cj.o tok_stream.o
	$(CC) -o tok_stream cj.o tok_stream.o

hash.o: hash.c cj.h
	$(CC) $(CFLAGS) -o hash.o hash.c

tok_stream.o: tok_stream.c cj.h
	$(CC) $(CFLAGS) -o tok_stream.o tok_stream.c

cj.o: cj.c cj.h
	$(CC) $(CFLAGS) -o cj.o cj.c

clean:
	rm -f hash tok_stream *.o
