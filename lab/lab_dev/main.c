#include <stdio.h>
#include <time.h>

#include "libs/libs.h"
#include "tree/tree.h"

int main() {

    /* Initializing variables */
    auto int n;
    auto tree t;
    Tree_init(&t);
    /* srand(time(NULL) / 2); */

    /* I/O flow */
    do {
        printf("Type n: ");
        scanf("%d", &n);
    } while (n <= 0);

    /* Main part */
    fillInTree(&t, n);
    printf("Old tree: \n");
    Tree_print(&t, node_print);

    deleteRand(&t);
    printf("\nNew tree: \n");
    Tree_print(&t, node_print);

    Tree_delete(&t);

    /* Returning value */
    return 0;
}