# Initializing variables
CC = gcc
CFLAGS = -Wall
LDFLAGS = -lpthread

# Main part
all: prg1 prg2 prg3

prg1: prg1.c
	$(CC) $(CFLAGS) prg1.c -o prg1 $(LDFLAGS)

prg2: prg2.c
	$(CC) $(CFLAGS) prg2.c -o prg2 $(LDFLAGS)

prg3: prg3.c
	$(CC) $(CFLAGS) prg3.c -o prg3 $(LDFLAGS)

clean:
	rm -f *.o prg1 prg2 prg3
