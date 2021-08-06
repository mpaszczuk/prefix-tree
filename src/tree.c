#include "tree.h"

static void rb_left_rotate(tree_t *tree, node_t *x);
static void rb_right_rotate(tree_t *tree, node_t *y);
static void rb_insert_fixup(tree_t *tree, node_t *z);
static void rb_transplant(tree_t *tree, node_t*u, node_t*v);
static void rb_delete_fixup(tree_t *tree, node_t *x);

void rb_left_rotate(tree_t *tree, node_t *x) {
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
void rb_right_rotate(tree_t *tree, node_t *y) {
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
void rb_insert(tree_t *tree, node_t *z) {
    node_t *y = tree->nil;
    node_t *x = tree->root;
    while (x != tree->nil) {
        y = x;
        if (compare(&z->key, &x->key) < 0 ) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == tree->nil) {
        tree->root = z;
    } else if (compare(&z->key, &y->key)< -1) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = tree->nil;
    z->right = tree->nil;
    z->color = RED;
}

void rb_insert_fixup(tree_t *tree, node_t *z){
    while (z->parent->color == RED){
        if(z->parent == z->parent->parent->left){
            node_t *y = z->parent->parent->right;
            if(y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else if(z == z->parent->right){
                z = z->parent;
                rb_left_rotate(tree, z);
            }
            else{
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rb_right_rotate(tree, z->parent->parent);
            }
        }
        else{
            //TODO to jest zjebane
            node_t *y = z->parent->parent->left;
            if(y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else if(z == z->parent->left){
                z = z->parent;
                rb_left_rotate(tree, z);
            }
            else{
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rb_right_rotate(tree, z->parent->parent);
            }
        }
    }
}

void rb_transplant(tree_t *tree, node_t*u, node_t*v){
    if (u->parent == tree->nil){ // tree->root == u
        tree->root = v;
    }
    else if(u == u->parent->left){
        u->parent->left = v;
    }
    else{
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void rb_delete_fixup(tree_t *tree, node_t *x){
    while ( x != tree->root && x->color ){
        if(x == x->parent->left){
            node_t *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rb_left_rotate(tree, x->parent);
                w = x->parent->right;
            }
            if(w->left->color == BLACK && w->right->color == BLACK){
                w->color = RED;
                x = x->parent;
            }
            else if(w->right->color == BLACK){
                w->left->color = BLACK;
                w->color = RED;
                rb_right_rotate(tree, w);
                w = x->parent->right;
            }
            else{
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rb_left_rotate(tree, x->parent);
                x = tree->root;
            }
        }
        else {
            node_t *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rb_left_rotate(tree, x->parent);
                w = x->parent->left;
            }
            if(w->left->color == BLACK && w->right->color == BLACK){
                w->color = RED;
                x = x->parent;
            }
            else if(w->left->color == BLACK){
                w->right->color = BLACK;
                w->color = RED;
                rb_right_rotate(tree, w);
                w = x->parent->left;
            }
            else{
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rb_left_rotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

node_t *rb_tree_minimum(tree_t *tree, node_t *x){
    while( x->left != tree->nil){
        x = x->left;
    }
    return x;
}

node_t *rb_tree_maximum(tree_t *tree, node_t *x){
    while( x->right != tree->nil){
        x = x->right;
    }
    return x;
}

void rb_delete(tree_t *tree, node_t* z){
    node_t *x = tree->nil;
    node_t *y = z;
    color_t y_original_color = y->color;
    if(z->left == tree->nil){
        x = z->right;
        rb_transplant(tree, z, z->right);
    }
    else if(z->right == tree->nil){
        x = z->left;
        rb_transplant(tree, z, z->left);
    }
    else {
        y = rb_tree_minimum(tree, z->right);
        y_original_color = y->color;
        x = y->right;
        if(y->parent == z){
            x->parent = y;
        }
        else{
            rb_transplant(tree, y, y->right);
            y->right->parent = y;
        }
        rb_transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if(y_original_color == BLACK){
        rb_delete_fixup(tree, x);
    }
}

node_t *rb_check(tree_t *tree, unsigned int ip){
    node_t *node = tree->root;
    while(node != tree->nil){
        ip_t key = {
            .base = ip,
            .mask = node->key.mask
        };
        if(compare (&node->key, &key)> 0){
            node = node->right;
        }
        else if(compare(&node->key, &key) < 0){
            node = node->left;
        }
        else{
            return node;
        }
    }
    return tree->nil;
}

node_t *rb_search(tree_t *tree, ip_t *ip, int(*compare)(ip_t*, ip_t*)){
    node_t *node = tree->root;
    while(node != tree->nil){
        if(compare (&node->key, ip)> 0){
            node = node->right;
        }
        else if(compare(&node->key, ip) < 0){
            node = node->left;
        }
        else{
            return node;
        }
    }
    return tree->nil;
}
