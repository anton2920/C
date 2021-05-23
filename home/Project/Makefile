# Initializing variables
CC = cc
CFLAGS = -Wall -std=c90
LDFLAGS = -ldialog -lncursesw -lm

all: prog

prog: main.o menu.o math.o tree.o menu2.o menu3.o
	$(CC) $(CFLAGS) main.o menu.o menu2.o menu3.o tree.o math.o -o main $(LDFLAGS)
dbg: main.o menu.o math.o tree2.o menu2.o menu3.o
	$(CC) $(CFLAGS) main.o menu.o menu2.o menu3.o tree2.o math.o -o dbg_main $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

menu.o: libs/menu.c
	$(CC) $(CFLAGS) -c libs/menu.c

menu2.o:libs/menu2.c
	$(CC) $(CFLAGS) -c libs/menu2.c

menu3.o: libs/menu3.c
	$(CC) $(CFLAGS) -c libs/menu3.c

math.o: libs/math.c
	$(CC) $(CFLAGS) -c libs/math.c

tree.o: libs/tree.o
	$(CC) $(CFLAGS) -c libs/tree.c

tree2.o: libs/tree2.o
	$(CC) $(CFLAGS) -c libs/tree2.c

clean:
	rm -rf *.o main dbg_main
