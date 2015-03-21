CC=g++
CFLAGS=-g -std=c++0x
LDFLAGS=-g



all: main.o tree.o token.o buffer.o
	$(CC) -o p1 main.o tree.o token.o buffer.o $(CFLAGS) $(LDFLAGS)

token.o: token.cpp token.h
	$(CC) -c token.cpp $(CFLAGS)

main.o: main.h main.cpp tree.h programstate.h token.h buffer.h
	$(CC) -c main.cpp $(CFLAGS)

buffer.o: buffer.cpp buffer.h programstate.h
	$(CC) -c buffer.cpp $(CFLAGS)

tree.o: tree.h tree.cpp treetypes.cpp programstate.h token.h
	$(CC) -c tree.cpp $(CFLAGS)

.phony clean:
	rm *.o p1
