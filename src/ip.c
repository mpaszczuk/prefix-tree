#include "ip.h"
#include "tree.h"
#include <stdlib.h>

static tree_t *tree;

void init_tree(tree_t *tree_) {
    tree = tree_;
}

int add(unsigned int base, char mask) {
    node_t * node;//    rb_search()
    if(node == tree->nil){
        node_t *new_node = (node_t *)malloc(sizeof (node_t));
        new_node->key.base = base;
        new_node->key.mask = mask;
        rb_insert(tree, new_node);
    }
    return 0;
}
int del(unsigned int base, char mask) {
    node_t * node;//    rb_search()
    if(node != tree->nil) {
        rb_delete(tree, node);
    }
}
char check(unsigned int ip) {
    node_t *ip_node; // = rb_check(tree, ip);
    if (ip_node != tree->nil) {
        return ip_node->key.mask;
    }
    return -1;
}

int compare(const ip_t *ip1, const ip_t *ip2) {
    unsigned int mask1 = 2 ^ ip1->mask - 1;
    unsigned int mask2 = 2 ^ ip2->mask - 1;
    if ((ip1->base & mask1) > (ip1->base & mask2)) {
        return 1;
    } else if ((ip1->base & mask1) < (ip1->base & mask2)) {
        return -1;
    } else {
        return 0;
    }
}
