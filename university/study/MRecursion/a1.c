#include <stdio.h>

int fib(int);

/* N-member of fibonacci sequence */
main() {

	/* Initializing variables */
	int n;

	/* I/O flow && VarCheck*/
	do {
		printf("Type N: ");
		scanf("%d", &n);
	} while (n <= 0);

	/* Final output */
	printf("The %d%s member of fibonacci sequence: %d\n", n, (n == 1) ? "st" : (n == 2) ? "nd" : (n == 3) ? "rd" : "th", fib(n));
}


int fib(int n) {
	
	/* Returning value */
	return (n == 1 || n == 2) ? 1 : fib(n - 1) + fib(n - 2);
}