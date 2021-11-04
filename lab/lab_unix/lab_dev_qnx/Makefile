# Initializing variables
export QNX_HOST=/usr/qnx632/host/qnx6/x86/
export QNX_TARGET=/usr/qnx632/target/qnx6/

CFLAGS = -Wall -O2 -I/usr/include
LDFLAGS =

# Main part
all: server client

server: server.c
	$(CC) $(CFLAGS) server.c -o server $(LDFLAGS)

client: client.c
	$(CC) $(CFLAGS) client.c -o client $(LDFLAGS)

clean:
	rm -f *.o server client
