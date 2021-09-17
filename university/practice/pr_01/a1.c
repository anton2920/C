/* gcc -Wall -std=c90 */

/* On the railway tracks worth road composition.
* It is necessary to find the largest consistent chain of tanks between freight cars.
* (car type set by number, input terminated by zero) */

#include <stdio.h>

main() {

    /* Initializing variables */
    int type, max_chain = 0, new = 0, chain = 0;
    /* type — reads, whether it's a tanker or a «cargo», max_chain — remembers maximum number of boys between girls, new — sort of flag, chain — remembers current length of chain */

    /* Main part */
    while (1) {
        do {
            printf("Type wagon type [0 — quit, 1 — tanker, 2 — cargo (freight car)]: ");
            scanf("%d", &type);
        } while (type < 0 || type > 2);
        if (type == 1 && new) {
            if (new == 2) {
                ++chain;
            } else {
                chain = 1;
                new = 2;
            }
        } else if (type == 2) {
            new = 1;
            if (chain > max_chain) {
                max_chain = chain;
            }
        } else if (!type) {
            break;
        }

    }

    /* Final output */
    printf("The maximum number is: %d\n", max_chain);
}