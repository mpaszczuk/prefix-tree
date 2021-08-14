#include "trie.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

node_t *new_node_t() {
    node_t *node = malloc(sizeof(node_t));
    node->child = malloc(2 * sizeof(node_t *));
    node->child[0] = node->child[1] = NULL;
    node->ip = NULL;
    node->parent = NULL;
    return node;
}

ip_t *new_ip_t(){
    ip_t *ip = (ip_t*)malloc(sizeof(ip_t));
    ip->base = 0;
    ip->mask = 0;
    return ip;
}

node_t *del_node_t(node_t* node){
    if(node->ip != NULL){
       free(node->ip) ;
       node->ip = NULL;
    }
    if(node->child != NULL){
        free(node->child) ;
        node->child = NULL;
    }
    if(node->parent != NULL){
       if(node->parent->child[LEFT_CHILD] == node){
            node->parent->child[LEFT_CHILD] = NULL;
       }
       else if(node->parent->child[RIGHT_CHILD] == node){
           node->parent->child[RIGHT_CHILD] = NULL;
       }
    }
}
/* Generate bitmask filed with '1' of the same length as ip->bitmask */
unsigned int get_bitmask(char mask){
    if(mask ==32){
        return UINT32_MAX;
    }
    return ((1<<mask) - 1) << (32 - mask);
}

/*Get bit of value at position bit_number*/
unsigned int get_bit(unsigned int value, unsigned char bit_number){
    return (value & (1 << (32 - bit_number - 1))) >> (32 - bit_number - 1);
}

node_t *trie_insert(trie_t *trie, ip_t *ip_) {
    node_t *node = trie_search(trie, ip_);
    if (node != NULL && node->ip != NULL) {
        return NULL;
    }
    /* Check if ip base has the same number of bits as bitmask. In case of differences
     * function returns NULL ptr. Mask should be the same length as ip */
    if(ip_->base != (ip_->base & get_bitmask(ip_->mask))){
        return NULL;
    }
    ip_t *ip = new_ip_t();
    ip->base = ip_->base;
    ip->mask = ip_->mask;
    if (trie->root == NULL) {
        trie->root = new_node_t();
        trie->root->parent = NULL;
    }
    /* If ip == 0.0.0.0 and bitmask == 0 then assign it to the root */
    if(ip->mask == 0){
        trie->root->ip = ip;
        return trie->root;
    }
    node_t *current = trie->root;
    /*Iterate through subsequent bits of ip  */
    for (unsigned int i = 0; i < ip->mask; ++i) {
        unsigned int bit = (ip->base & (1 << (32 - i - 1))) >> (32 - i - 1);
        /* Check if child on specific position exists and if needed create new node */
        if (current->child[bit] == NULL) {
            current->child[bit] = new_node_t();
            current->child[bit]->parent = current;
        }
        current = current->child[bit];
    }
    current->ip = ip;
    return current;
}

void node_deinit(node_t *node) {
    if(node == NULL){
        return;
    }
    for (int i = 0; i < MAX_NUMBER_OF_CHILDREN; ++i) {
        if (node->child[i] != NULL) {
            node_deinit(node->child[i]);
            node->child[i] = NULL;
        }
    }
    if (node->ip != NULL) {
        free(node->ip);
        node->ip = NULL;
    }
    free(node);
}

void trie_deinit(trie_t *trie) {
    node_deinit(trie->root);
    free(trie);
}

node_t *trie_search(trie_t *trie, ip_t *ip) {
    node_t *current = trie->root;
    if (current == NULL) {
        return NULL;
    }
    for (int i = 0; i < ip->mask; ++i) {
        unsigned int bit = get_bit(ip->base, i);
        if (current->child[bit] == NULL) {
            return NULL;
        }
        current = current->child[bit];
    }
    return current;
}

node_t *trie_check(trie_t *trie, unsigned int ip) {
    node_t *current = trie->root;
    node_t *previous = NULL;
    for (unsigned int i = 0; current != NULL; ++i) {
        previous = current;
        unsigned int bit = get_bit(ip, i);
        current = current->child[bit];
    }
    while (previous != NULL && previous->ip == NULL) {
        previous = previous->parent;
    }
    return previous;
}

int trie_delete(trie_t *trie, ip_t *ip) {
    node_t *current = trie->root;
    for (unsigned char i = 0; i < ip->mask; ++i) {
        unsigned int bit = get_bit(ip->base, i);
        if (current->child[bit] == NULL) {
            return -1;
        }
        current = current->child[bit];
    }
    if (current->ip) {
        free(current->ip);
        current->ip = NULL;
    } else {
        return -1;
    }
    node_t *parent;
    /* Remove nodes in branch without any child and ip. */
    while (current != NULL && current->ip == NULL && !current->child[LEFT_CHILD] && !current->child[RIGHT_CHILD]) {
        parent = current->parent;
        if(current == trie->root){
            trie->root = NULL;
        }
        del_node_t(current);
        current = parent;
    }
    return 0;
}

node_t *trie_previous(node_t *node) {
    if (node->child[0] != NULL) {
        node = node->child[0];
        while (node->child[1] != NULL) node = node->child[1];
        return node;
    }
    while (node->parent != NULL && node->parent->child[0] == node) {
        node = node->parent;
    }
    return node->parent;
}

node_t *trie_next(node_t *node) {
    if (node->child[1] != NULL) {
        node = node->child[1];
        while (node->child[0] != NULL) node = node->child[0];
        return node;
    }
    while (node->parent != NULL && node->parent->child[1] == node) {
        node = node->parent;
    }
    return node->parent;
}

int COUNT = 10;

void trie_print(node_t *root, int space) {
    if (root == NULL) {
        return;
    }
    // Increase distance between levels
    space += COUNT;

    // Process right child first
    trie_print(root->child[1], space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++) {
        printf(" ");
    }
    unsigned int val;
    if (root->parent != NULL)
        val = root->parent->child[0] == root ? 0 : 1;
    else
        val = 20;
    if (root->ip != NULL)
        printf("%d ip=%u mask=%d\n", val, root->ip->base, root->ip->mask);
    else
        printf("%d \n", val);
    // Process left child
    trie_print(root->child[0], space);
}