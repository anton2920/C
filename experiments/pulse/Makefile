# Initialising variables
CC = gcc
CFLAGS = -O2 -std=c90 -Wall -Wextra
LDFLAGS = -lpulse -lpulse-simple

# Main part
all: pulse_test

pulse_test pulse_test.c:
	$(CC) pulse_test.c -o pulse_test $(LDFLAGS)

clean:
	rm -f *.o pulse_test
