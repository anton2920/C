# Initializing variables
CC = gcc
CFLAGS = -Wall
LDFLAGS = -lpthread -lrt

# Main part
all: prg2 client

prg2: prg2.c libs/libs.c libs/libs.h
	$(CC) $(CFLAGS) prg2.c libs/libs.c -o prg2 $(LDFLAGS)

client: client.c libs/libs.c libs/libs.h
	$(CC) $(CFLAGS) client.c libs/libs.c -o client $(LDFLAGS)	

clean:
	rm -f *.o prg2 client
