# Initializing variables
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c90
LDFLAGS = -lSTL

# Main part
all: main algorithms visuals proceses
	$(CC) $(CFLAGS) main.o libs.o visual.o process.o -o main $(LDFLAGS)

main: main.c
	$(CC) $(CFLAGS) -c main.c

algorithms: libs/libs.c libs/libs.h
	$(CC) $(CFLAGS) -c libs/libs.c

visuals: visual/visual.c visual/visual.h
	$(CC) $(CFLAGS) -c visual/visual.c

proceses: process/process.c process/process.h
	$(CC) $(CFLAGS) -c process/process.c

clean:
	rm -f *.o main