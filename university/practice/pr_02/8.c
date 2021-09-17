/* gcc -Wall -std=c90 */
#include <stdio.h>

#define SIZE 1024

void preobr(double *, const int *, const int *);
int pow_2(int);
void copy(double *, double *, const int *);

main() {

    /* Initializing variables */
    double mas[SIZE];
    int i, n, flag, j;

    /* I/O flow */
    do {
        printf("Type the number of elements: ");
        scanf("%d", &n);
        for (i = 0; ; ++i){
            if (n > pow_2(i)) {
                continue;
            } else if (n == pow_2(i)) {
                flag = 1;
                break;
            } else {
                flag = 0;
                break;
            }
        }
    } while (!flag);
    j = i;

    for (i = 0; i < n; ++i) {
        printf("Type %d%s element: ", i, (i == 1) ? "st" : (i == 2) ? "nd" : (i == 3) ? "rd" : "th");
        scanf("%lf", mas + i);
    }

    /* Final output */
    printf("New array: ");
    preobr(mas, &n, &j);
    for (i = 0; i < n; ++i) {
        printf("%.1lf ", *(mas + i));
    }
    printf("\n");
}

void preobr(double *mas, const int *n, const int *j) {

    /* Initializing variables */
    int i, start = 0, end = *n - 1, temp, k, m;
    double tmp[SIZE];

    /* Main part */
    for (i = 0; i < *j; ++i) {
        temp = (start + end) / 2;
        copy(mas, tmp, n);
        for (k = 0, m = 0; k <= temp; ++k, m += 2) {
            mas[k] = (tmp[m] + tmp[m + 1]) / 2;
        }
        for (k = temp + 1, m = 0; k <= end; ++k, m += 2) {
            mas[k] = (tmp[m] - tmp[m + 1]) / 2;
        }
        end = temp;
    }
}

int pow_2(int n) {

    /* Initializing variables */
    int res = 1, i;

    /* Main part */
    for (i = 0; i < n; ++i) {
        res *= 2;
    }

    /* Returning value */
    return res;
}

void copy(double *mas1, double *mas2, const int *n) {

    /* Initializing variables */
    int i;

    for (i = 0; ; ++i) {
        *mas2++ = *mas1++;
        if (i == *n) {
            break;
        }
    }
}