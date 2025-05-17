CC=clang
CFLAGS=-Wall -g -c

all: hash tok_stream tokenize parse

parse: parse.o
	$(CC) -o parse parse.o

tokenize: tokenize.o
	$(CC) -o tokenize tokenize.o

hash: hash.o
	$(CC) -o hash hash.o

tok_stream: tok_stream.o
	$(CC) -o tok_stream tok_stream.o

parse.o: tests/parse.c json.h
	$(CC) $(CFLAGS) -o parse.o tests/parse.c

hash.o: tests/hash.c json.h
	$(CC) $(CFLAGS) -o hash.o tests/hash.c

tokenize.o: tests/tokenize.c json.h
	$(CC) $(CFLAGS) -o tokenize.o tests/tokenize.c

tok_stream.o: tests/tok_stream.c json.h
	$(CC) $(CFLAGS) -o tok_stream.o tests/tok_stream.c

clean:
	rm -f hash tok_stream tokenize parse *.o
