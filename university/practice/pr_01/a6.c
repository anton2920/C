/* gcc -Wall -std=c90 */

/* At the counter in the store lined up a queue of buyers.
 * Service time by the seller of each customer is entered from the keyboard (0 is a sign of the end of the queue).
 * Get the time each customer is in the queue. Specify the number of the buyer, for the service which the seller took the shortest time. */

#include <stdio.h>

main() {

    /* initializing variables */
    int n, nsum = 0, nmin = -1, cus_num = 1, cus_num_min;
    /* n — time for customer, nsum — total time, nmin — minimum time */

    /* Main part */

    do {
        printf("Type n (amount of time for a customer): ");
        scanf("%d", &n);
        if (!n) {
            break;
        }
        if (nmin == -1) {
            nmin = n;
            cus_num_min = cus_num;
        } else if (n < nmin) {
            nmin = n;
            cus_num_min = cus_num;
        }
        printf("He stands in queue %d\n\n", nsum);
        nsum += n;
        ++cus_num;
    } while (1);

    printf("\nThe customer with the least time is %d\n", cus_num_min);
}