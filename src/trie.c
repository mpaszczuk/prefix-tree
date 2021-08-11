#include "trie.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

node_t * new_node_t()
{
    node_t* node = malloc(sizeof(node_t));
    node->child = malloc(2 * sizeof(node_t*));
    node->child[0] = node->child[1] = NULL;
    return node;
}

int trie_insert(trie_t *trie, ip_t *ip) {
    if (trie->root == NULL) {
        trie->root = new_node_t();
        trie->root->parent = NULL;
    }
    //        if(count == 0)				//-------------If Subnet Mask is 0.0.0.0-----------------
    //        {
    //            head->isend = true;
    //            head->data = d;
    //        }
    node_t *current = trie->root;
    for (unsigned int i = 0; i < ip->mask; ++i) {
        unsigned int val = (ip->base & 1 << (32 - i - 1)) >> (32 - i - 1);
        if (current->child[val] == NULL) {
            current->child[val] = new_node_t();
            current->child[val]->parent = current;
        }
        current = current->child[val];
    }
    current->ip = ip;
}

void node_deinit(node_t* node){
    for(int i =0; i <2; ++i){
        if(node->child[i] != NULL) {
            node_deinit(node->child[i]);
        }
    }
    if(node->ip != NULL){
        free(node->ip);
    }
    free(node);
}

void trie_deinit(trie_t * trie){
    node_deinit(trie->root);
    free(trie);
}



node_t *trie_search(trie_t *trie, ip_t *ip) {
    node_t *current = trie->root;
    if(current == NULL){
        return NULL;
    }
    for (int i = 0; i < ip->mask; ++i) {
        unsigned int val = (ip->base & (2 << (32 - i-1))) >> (32 - i-1);
        if (current->child[val] == NULL) {
            return NULL;
        }
        current = current->child[val];
    }
    return current;
}

node_t *trie_check(trie_t *trie, unsigned int ip) {
    node_t *current = trie->root;
    node_t *previous = NULL;
    for (unsigned int i = 1; current != NULL; ++i) {
        previous = current;
        unsigned int val = (ip & (1 << (32 - i))) >> (32 - i);
        current = current->child[val];
    }
    while (previous != NULL && previous->ip == NULL) {
        previous = previous->parent;
    }
    return previous;
}

int trie_delete(trie_t *trie, ip_t *ip) {
    node_t *current = trie->root;
    for (unsigned int i = 0; i < ip->mask; ++i) {
        unsigned int val = (ip->base & (1 << (32 - i - 1))) >> (32 - i - 1);
        if (current->child[val] == NULL) {
            return -1;
        }
        current = current->child[val];
    }
    if (current->ip) {
        free(current->ip);
        current->ip = NULL;
    } else {
        return -1;
    }
    node_t *parent;
    while (!current->child[0] && !current->child[1]) {
        parent = current->parent;
        free(current);
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

int COUNT=10;

void trie_print(node_t *root, int space) {
    if (root == NULL){
        return;
    }
    // Increase distance between levels
    space += COUNT;

    // Process right child first
    trie_print(root->child[1], space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++){
        printf(" ");
    }
    unsigned int val;
    if(root->parent != NULL)
        val = root->parent->child[0] == root ? 0:1;
    else
        val = 20;
    if(root->ip != NULL)
        printf("%d ip=%u mask=%d\n",val , root->ip->base, root->ip->mask);
    else
        printf("%d \n", val);
    // Process left child
    trie_print(root->child[0], space);
}