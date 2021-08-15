#include "test_utils.h"
#include <stdio.h>
#include "ip.h"

int COUNT = 10;

unsigned int ip_to_int(unsigned char ip1, unsigned char ip2, unsigned char ip3, unsigned char ip4) {
    return (unsigned int) (ip1 << 24) | (ip2 << 16) | (ip3 << 8) | ip4;
}

bool check_parent_child_relation(node_t *parent, node_t *child, int child_number) {
    /* Check parent-child and child-parent relation */
    if (child->parent != parent || parent->child[child_number] != child) {
        return false;
    }
    return true;
}

bool check_node_ip_and_mask(node_t *node) {
    if (node == NULL) {
        return false;
    }
    unsigned int ip = 0;
    char mask;
    node_t *current = node;
    for (mask = 0; mask < IP_LENGTH; ++mask) {
        if (current->parent != NULL) {
            unsigned int tmp = current->parent->child[LEFT_CHILD] == current ? LEFT_CHILD : RIGHT_CHILD;
            ip |= tmp << mask;
            current = current->parent;
        } else {
            break;
        }
    }
    ip = ip << (IP_LENGTH - mask);
    return node->ip->base == ip && node->ip->mask == mask;
}

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
