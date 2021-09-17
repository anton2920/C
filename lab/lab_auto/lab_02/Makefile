# Initializing variables
CC = gcc
CFLAGS = -std=c90 -O2 -Wall -Wextra
LDFLAGS = -lSTL

# Main part
all: main.c
	$(CC) $(CFLAGS) main.c -o main $(LDFLAGS)

clean:
	rm -f *.o main