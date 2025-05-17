CC=clang
CFLAGS=-Wall -g -c

all: hash tok_stream tokenize parse

parse: parse.o cj.o
	$(CC) -o parse cj.o parse.o

tokenize: cj.o tokenize.o
	$(CC) -o tokenize tokenize.o cj.o

hash: cj.o hash.o
	$(CC) -o hash cj.o hash.o

tok_stream: cj.o tok_stream.o
	$(CC) -o tok_stream cj.o tok_stream.o

parse.o: tests/parse.c cj.h
	$(CC) $(CFLAGS) -o parse.o tests/parse.c

hash.o: tests/hash.c cj.h
	$(CC) $(CFLAGS) -o hash.o tests/hash.c

tokenize.o: tests/tokenize.c cj.h
	$(CC) $(CFLAGS) -o tokenize.o tests/tokenize.c

tok_stream.o: tests/tok_stream.c cj.h
	$(CC) $(CFLAGS) -o tok_stream.o tests/tok_stream.c

cj.o: cj.c cj.h
	$(CC) $(CFLAGS) -o cj.o cj.c

clean:
	rm -f hash tok_stream tokenize parse *.o
