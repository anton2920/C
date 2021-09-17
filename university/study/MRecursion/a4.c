#include <stdio.h>

int st(int);

main() {

	/* Initializing variables */
	int n = 5;

	/* Final output */
	printf("For %d stairs: %d\n", n, st(n));

}

int st(int n) {

	/* Initializing variables */
	int k = 0;

	/* Main part */
	if (!n)
		return 1;
	k += st(n - 1);
	if (n >= 2) 
		k += st(n - 2);
	if (n >= 3)
		k += st(n - 3);
	
	/* Returning value */
	return k;
}