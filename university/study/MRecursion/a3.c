#include <stdio.h>

#define ABS(X) ((X) < 0) ? (-X) : X
#define EPS 0.00001

double k(double, double, double);

main() {

	/* Initializing variables */
	double x = 25;

	/* Final output */
	printf("Sqrt(%lf) = %lf\n", x, k(0, x, x));

}

double k(double a, double b, double x) {

	/* Initializing variables */
	double c = (a + b) / 2, d = b - a;

	/* Main part */
	if (c * c == x)
		return c;
	if (ABS(d) < EPS) 
		return c;
	if (c * c < x) {
		return k(c, b, x);
	} else {
		return k(a, c, x);
	}

}