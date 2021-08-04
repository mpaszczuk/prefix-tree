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
    node_t *nil = NULL;
    node_t *root;
} tree_t;

void left_rotate(tree_t * tree, node_t *x) {
   node_t *y = x->right;
   x->right = y->left;
   if(y->left != tree->nil){
       y->left->parent = x;
   }
   y->parent = x->parent;
   if(x->parent == tree->nil){
       tree->root = y;
   }
   else if (x == x->parent->left){
       x->parent->left = y;
   }
   else {
       x->parent->right = y;
   }
   y->left = x;
   x->parent = y;
}