# Initializing variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c90 -O2
LDFLAGS = 

# Main part
all: server client

server: server.c
	$(CC) $(CFLAGS) server.c -o server $(LDFLAGS)

client: client.c
	$(CC) $(CFLAGS) client.c -o client $(LDFLAGS)

clean:
	rm -f *.o server client
