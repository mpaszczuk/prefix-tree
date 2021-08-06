#pragma once

#include "ip.h"

typedef enum {
    RED,
    BLACK
} color_t;


typedef struct node_t node_t;

typedef struct {
    node_t *nil;
    node_t *root;
} tree_t;

struct node_t {
    color_t color;
    node_t *parent;
    unsigned int key;
    tree_t *subtree;
    node_t *left;
    node_t *right;
};
void rb_insert(tree_t *tree, node_t *z);
void rb_delete(tree_t *tree, node_t* z);
node_t *rb_tree_minimum(tree_t *tree, node_t *x);
