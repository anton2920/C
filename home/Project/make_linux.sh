gcc -std=c90 -Wall -c libs/menu.c && gcc -std=c90 -Wall -c libs/menu2.c && gcc -std=c90 -Wall -c libs/menu3.c && gcc -std=c90 -Wall -c main.c && gcc -std=c90 -Wall -c libs/math.c && gcc -std=c90 -Wall -c libs/tree.c && gcc -std=c90 -Wall main.o menu.o menu2.o menu3.o math.o tree.o -o main -ldialog -lncursesw -lm
