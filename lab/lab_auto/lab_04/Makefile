# Initializing variables
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c90
LDFLAGS = -lSTL

# Main part
all: main hash_map
	$(CC) $(CFLAGS) main.o hash_map.o -o main $(LDFLAGS)

main: main.c
	$(CC) $(CFLAGS) -c main.c

hash_map: hash_map.c hash_map.h libs.h
	$(CC) $(CFLAGS) -c hash_map.c

clean:
	rm -f *.o main
