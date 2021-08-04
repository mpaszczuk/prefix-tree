#pragma once

enum {
    red,
    black
} color_t;

typedef struct {
    int ip;
    int mask;

} ip_t;

typedef struct {
    color_t color;
    node_t *parent;
    ip_t key;
    node_t *left;
    node_t *right;
} node_t;

typedef struct {
    node_t *nil;
    node_t *root;
} tree_t;

void left_rotate(tree_t *tree, node_t *x) {
    node_t *y = x->right;
    x->right = y->left;
    if (y->left != tree->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void right_rotate(tree_t *tree, node_t *y) {
    node_t *x = y->left;
    y->left = x->right;
    if (x->right != tree->nil) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == tree->nil) {
        tree->root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}
