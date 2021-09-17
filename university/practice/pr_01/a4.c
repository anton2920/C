/* gcc -Wall -std=c90 */

/* You owe your friend n rubles. Instead of asking you to repay the entire debt to him at once, your friend suggests that you pay him a certain amount every week for m weeks.
 * He asks you to pay him a penny in the first week, 2 in the second, 4 in the third, 8 in the fourth, and so on.
 * You have almost agreed with his offer, but then decide to first write a program to check how much you will have to pay if you accept his offer.
 * Write this program and decide which way is best to pay your debt. */

#include <stdio.h>

int bin_pw(int pow); /* Raise 2 to the (pow - 1)'s power */

main() {

    /* Initializing variables */
    int num_week, debt;

    /* I/O flow */
    do {
        printf("Type debt (in roubles): ");
        scanf("%d", &debt);
    } while (debt <= 0);
    do {
        printf("Type the number of weeks: ");
        scanf("%d", &num_week);
    } while (num_week < 1);

    /* Final output */
    printf("%s\n", (bin_pw(num_week) - 1 > debt * 100) ? "It's better to pay him a full sum" : "It's better to agree with your friend's plan");
}

int bin_pw(int pow){

    /* Initializing variables */
    int i, ans = 1, lpow = 1;

    /* Main part */
    for (i = 1; i < pow; ++i) {
        ans += lpow *= 2;
    }

    /* Returning value */
    return ans;
}