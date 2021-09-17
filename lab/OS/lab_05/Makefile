# Initializing variables
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c90
LDFLAGS = -lSTL

# Main part
all: main algorithms visual process
	$(CC) $(CFLAGS) main.o libs.o visual.o process.o -o main $(LDFLAGS)

main: main.c
	$(CC) $(CFLAGS) -c main.c

algorithms: libs.c libs.h
	$(CC) $(CFLAGS) -c libs.c

visual: visual.c visual.h
	$(CC) $(CFLAGS) -c visual.c

process: process.c process.h
	$(CC) $(CFLAGS) -c process.c

clean:
	rm -f *.o main