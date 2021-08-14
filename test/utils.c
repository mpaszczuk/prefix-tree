#include "utils.h"
#include <stddef.h>
#include <stdio.h>

int COUNT = 10;

void trie_print(node_t *root, int space) {
    if (root == NULL) {
        return;
    }
    /*Increase distance between levels*/
    space += COUNT;

    /*Process right child first*/
    trie_print(root->child[1], space);

    /*Print current node after space*/
    printf("\n");
    for (int i = COUNT; i < space; i++) {
        printf(" ");
    }
    unsigned int val;
    if (root->parent != NULL)
        val = root->parent->child[LEFT_CHILD] == root ? LEFT_CHILD : RIGHT_CHILD;
    else
        val = 20;
    if (root->ip != NULL)
        printf("%u ip=%u mask=%d\n", val, root->ip->base, root->ip->mask);
    else
        printf("%u \n", val);
    /*Process left child*/
    trie_print(root->child[LEFT_CHILD], space);
}
