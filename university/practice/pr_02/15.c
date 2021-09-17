/* gcc -Wall -std=c90 */
#include <stdio.h>

#define SIZE 1000

/* Write a program that each element of the sequence additionally repeats as many times as it was before.
 * 38 | Arrays and collections For example, the sequence (1, 2, 1, 2, 1) will be converted to (1, 2, 1, 1, 2, 2, 1, 1, 1).
 */

main() {

    /* Initializing variables */
    struct element {
        int value;
        int count;
    };
    struct element mas[SIZE];
    int i, flag = 0, j;

    /* I/O flow */
    for (i = 0; ; ++i) {
        printf("Type %d%s element: ", i, (i == 1) ? "st" : (i == 2) ? "nd" : (i == 3) ? "rd" : "th");
        scanf("%d", &(mas[i].value));
        if (mas[i].value == 0) {
            mas[i].count = 0;
            break;
        }
        for (j = i - 1; j >= 0; --j) {
            if (mas[j].value == mas[i].value) {
                flag = 1;
                break;
            } else {
                flag = 0;
            }
        }
        if (!flag) {
            mas[i].count = 1;
        } else {
            mas[i].count = mas[j].count + 1;
        }
    }

    /* Final output */
    printf("Final sequence: ");
    for (i = 0; mas[i].count && mas[i].value; ++i) {
        for (j = 0; j < mas[i].count; ++j) {
            printf("%d ", mas[i].value);
        }
    }
    printf("\n");
}