CC = gcc
CFLAGS = -Isrc/include
LDFLAGS = -Lsrc/lib -lmingw32 -lSDL2main -lSDL2

all: main.o
	$(CC) -o maze main.o $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c