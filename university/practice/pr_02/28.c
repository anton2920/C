/* gcc -Wall -std=c90 */
#include <stdio.h>

#define SIZE 201

/* Given an ordered array of n> 100 elements calculated by the formula: Ai = i * 0.5 + 2 * i * i + 7.
 * Insert the number B (entered from the keyboard) so that the ordering of the array is not broken.
 * Determine whether there is an element in the array equal to C (keyboard input) using the dichotomy method
 */

/* Functions declarations */
int search(const double *, const int *);
int menu2(void);


main() {

    /* Initializing variables */
    double mas[SIZE];
    int i, n, b, temp, flag, flag2 = -1;

    /* I/O flow */
    do {
        printf("Type size of array [101..%d]: ", SIZE - 1);
        scanf("%d", &n);
    } while (n <= 100 || n > (SIZE - 1));

    /* Main part */
    printf("Array: ");
    for (i = 0; i < n; ++i) {
        mas[i] = i * 0.5 + 2 * i * i + 7;
        printf("%.1lf ", *(mas + i));
    }
    printf("\n");

    /* Searching for C */
    while (1) {
        if ((flag2 = menu2())) {
            if ((flag = search(mas, &n))) {
                printf("The element C exists in array\n");
            } else {
                printf("The element C doesn't exist ia array\n");
            }
        } else {
            break;
        }

    }

    /* Inserting B */
    printf("Type B: ");
    scanf("%d", &b);
    for (i = 1; i < n; ++i) {
        if (*(mas + (i - 1)) < b && *(mas + i) > b) {
            temp = i;
            for (i = n; i > temp - 1; --i) {
                mas[i + 1] = mas[i];
            }
            mas[temp] = b;
            break;
        } else if (b >= *(mas + (n - 1))) {
            mas[n] = b;
            break;
        }
    }
    printf("New array: ");
    for (i = 0; i <= n; ++i) {
        printf("%.1lf ", *(mas + i));
    }
    printf("\n");

    /* Searching for C #2 */
    while (1) {
        if ((flag2 = menu2())) {
            if ((flag = search(mas, &n))) {
                printf("The element C exists in array\n");
            } else {
                printf("The element C doesn't exist ia array\n");
            }
        } else {
            break;
        }

    }

}

int search(const double *mas, const int *n) {

    /* Initializing variables */
    int start, end = (*n) - 1, temp, flag = 0;
    double c;

    /* I/O flow */
    printf("Type C: ");
    scanf("%lf", &c);

    /* Main part */
    start = 0;
    while (1) {
        temp = (start + end) / 2;
        if (*(mas + temp) == c) {
            flag = 1;
            break;
        } else if (start == end || (end - start) == 1) {
            flag = 0;
            break;
        } else if (*(mas + temp) > c) {
            end = temp;
        } else if (*(mas + temp) < c) {
            start = temp;
        } else {
            flag = 0;
            break;
        }
    }

    /* Returning value */
    return flag;
}

int menu2(void) {

    /* Initializing variables */
    int func, junk;

    /* I/O flow */
    while ((junk = getchar()) != '\n')
        ;
    while (1) {
        printf("Do you want to search for C? [Y/n]: ");
        switch ((func = getchar())) {
            case 'y': case 'Y': case '\n':
                return 1;
            case 'n': case 'N':
                while ((junk = getchar()) != '\n')
                    ;
                return 0;
            default:
                while ((junk = getchar()) != '\n')
                    ;
                continue;
        }
    }
}