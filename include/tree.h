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
        if (z->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == tree->nil) {
        tree->root = z;
    } else if (z->key < y->key) {
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