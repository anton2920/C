#include <stdio.h>
#include "input.h"

main() {

	/* Initializing variables */
	int a;
	float b;

	/* I/O flow */
	input("Type an integer: ", &a);
	printf("A = %d\n", a);
	finput("Type a float: ", &b);
	printf("B = %f\n", b);
}