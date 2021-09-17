/* gcc -Wall -std=c90 */

/* Write a program to determine the number of 2 * N-digit tickets, in which the sum of the first N decimal digits is equal to the sum of the N last decimal digits;
 * in this case, N is an arbitrary natural number. */

#include <stdio.h>

int num_gen(int n);
int sum_dig(int n);
/* char *num_concat(int n, int x, int y); */

main() {

    /* Initializing variables */
    int n, i, temp, j, count = 0, jmax;

    /* I/O flow */
    do {
        printf("Type N: ");
        scanf("%d", &n);
    } while (n <= 0);

    /* Main part */
    printf("Tickets: ");
    for (i = 0; i <= num_gen(n); ++i) {
        temp = sum_dig(i);
        jmax = num_gen(n);
        for (j = 0; j <= jmax; ++j) {
            if (temp == sum_dig(j)) {
                ++count;
            }
        }
    }

    /* Final output */
    printf("The number of those tickets is: %d\n", count);

}

int num_gen(int n) {

    /* Initializing variables */
    int i, num = 0;

    /* Main part */
    for (i = 0; i < n - 1; ++i) {
        num += 9;
        num *= 10;
    }
    num += 9;

    /* Returning value */
    return num;
}

int sum_dig(int n) {

    /* Initializing variables */
    int sum = 0;

    /* Main part */
    while (n){
        sum += n % 10;
        n /= 10;
    }

    /* Returning value */
    return sum;
}

/* char *num_concat(int n, int x, int y) {

   Initializing variables
   int i, ten = 1;
   char str[11];

   Main part
   for (i = 0; i < n; ++i) {
      ten = ten * 10;
   }
   x *= ten;
   x += y;
   for (i = 2 * n; i >= 0; --i) {
       str[i] = x % 10;
       x /= 10;
   }
   str[2 * n + 1] = '\0';

   Returning value
   return str;

} */