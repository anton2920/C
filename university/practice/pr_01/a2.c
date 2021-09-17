/* gcc -Wall -std=c90 */

/* In the series are boys and girls.
* Determine the maximum number of boys standing between girls.
* (at input: boy-1, girl - 2, 0 - end of input) */
#include <stdio.h>

main() {

    /* Initializing variables */
    int type, max_chain = 0, new = 0, chain = 0;
    /* type — reads, whether it's a guy or a girl, max_chain — remembers maximum number of boys between girls, new — sort of flag, chain — remembers current length of chain */

    /* Main part */
    while (1) {
        do {
            printf("Type gender type [0 — end, 1 — guy, 2 — girl]: ");
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