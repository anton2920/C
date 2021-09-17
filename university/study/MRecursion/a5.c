#include <stdio.h>

int k(int, int);

main() {

	/* Initializing variables */

	/* Final output */
	printf("Number of sh*t: %d\n", k(0, 2));
}

int k(int a, int n) {

	/* Initializing variables */
	int c = 0;

	/* Main part */
	if (10000 <= a && a <= 99999) {
		if (!(a % n))
			return 1;
		else
			return 0;
		
	} else {
		c += k(a * 10 + 2, n);
		c += k(a * 10 + 3, n);
		c += k(a * 10 + 5, n);
		return c;
	}
}