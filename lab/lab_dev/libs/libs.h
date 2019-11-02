#ifndef LAB_DEV_LIBS_H
#define LAB_DEV_LIBS_H

/* Header inclusions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tree/tree.h"

/* Macro definitions */

/* Type definitions */
enum way {
    LEFT,
    RIGHT,
    STAND
};

/* Functions declarations */
void fillInTree(tree *, int);
void node_print(const tree_node *);
void deleteRand(tree *);

#endif
