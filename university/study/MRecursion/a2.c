#include <stdio.h>

#define ABS(X) ((X) < 0) ? (-X) : (X)
#define EPS 0.00001

double f(double);
double dih(double, double);

main() {

    /* Initializing variables */
    double a = -100, b = 100;

    /* Final output */
    if (f(a) * f(b) > 0) {
        printf("No roots!\n");
    } else {
        printf("x = %lf\n", (f(a) == 0) ? a : (f(b) == 0) ? b : dih(a, b));
    }

}

double f(double x) {

    /* Returning value */
    return 2 * x * x * x;
}

double dih(double a, double b) {

    /* Initializing variables */
    double c = (a + b) / 2, d = b - a;

    /* Returning value */
    return (f(c) == 0 || ABS(d) < EPS) ? c : (f(a) * f(c) < 0) ? dih(a, c) : dih(c, b);
}