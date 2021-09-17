/* gcc -Wall -std=c90 -O2 */

#include <stdio.h>

/* Write a program for finding all perfect and friendly numbers in a given interval (from 1 to 104).
A perfect is a number that is equal to the sum of all its divisors, except for itself (for example: 28 = 1 + 2 + 4 + 7 + 14).
Friendly - a pair of natural numbers M and N, for which the sum of all divisors of the number M (except M) is equal to N, and the sum of all divisors of the number N (except N) is equal to M. */

int div_sum(int num);

main() {

    /* Initializing variables */
    int start, end, i, j, temp;

    /* I/O flow && VarCheck */
    do {
        printf("Type «start» number: ");
        scanf("%d", &start);
    } while (start <= 0);
    do {
        printf("Type «end» number: ");
        scanf("%d", &end);
    } while (end <= start);

    /* Main part */

    /* Perfect numbers */
    printf("Perfect numbers: ");
    for (i = start; i <= end; ++i) {
        if (div_sum(i) == i) {
            printf("%d ", i);
        }
    }
    printf("\nFriendly numbers: ");
    for (i = start; i <= end; ++i) {
        temp = div_sum(i);
        for (j = i + 1; j < end; ++j) {
            if (div_sum(j) == i && temp == j) {
                printf("[%d, %d] ", i, j);
            }
        }
    }

    /* Final output */
    printf("\n");

}

int div_sum(int num) {

    /* Initializing variables */
    int sum = 0, i;

    /* Main part */
    for (i = 1; i <= num / 2; ++i) {
        if (num % i == 0) {
            sum += i;
        }
    }

    /* Returning value */
    return sum;
}