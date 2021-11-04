# Initializing variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c90 -O2
LDFLAGS =

# Main part
all: main

main: main.c
	$(CC) $(CFLAGS) main.c -o main $(LDFLAGS)

clean:
	rm -f *.o main
