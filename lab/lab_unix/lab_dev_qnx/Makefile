# Initializing variables
export QNX_HOST=/usr/qnx632/host/qnx6/x86/
export QNX_TARGET=/usr/qnx632/target/qnx6/

CFLAGS = -Wall -O2 -I/usr/include
LDFLAGS =

# Main part
all: main

main: main.c
	$(CC) $(CFLAGS) main.c -o main $(LDFLAGS)

clean:
	rm -f *.o main
