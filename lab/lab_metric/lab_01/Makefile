# Initializing variables
CC=gcc
CFLAGS=-I/usr/include/glib-2.0/ -I/usr/lib64/glib-2.0/include/ -Wall -Wextra -O2 -std=gnu90
LDFLAGS=-lglib-2.0

# Main part
all: main.c
	$(CC) $(CFLAGS) main.c -o main $(LDFLAGS)

clean:
	rm -f main
