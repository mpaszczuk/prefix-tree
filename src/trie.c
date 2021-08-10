#include "trie.h"
#include <stddef.h>
#include <stdlib.h>

int trie_insert(trie_t *trie, ip_t *ip) {
    if (trie->root == NULL) {
        trie->root = (node_t *) malloc(sizeof(node_t));
        trie->root->parent = NULL;
    }
//        if(count == 0)				//-------------If Subnet Mask is 0.0.0.0-----------------
//        {
//            head->isend = true;
//            head->data = d;
//        }
    node_t *current = trie->root;
    for(int i=0; i<ip->mask; ++i){
        int val = (ip->mask & 2<<(32-i)) >> (32-1);
        if(current->child[val] == NULL){
            current->child[val] = (node_t*)malloc(sizeof(node_t));
            current->child[val]->parent = current;
        }
        current = current->child[val];
    }
    current->ip = ip;
}

node_t *trie_search(trie_t *trie, ip_t *ip) {
    node_t *current = trie->root;
    for(int i=0; i<ip->mask; ++i){
        int val = (ip->mask & 2<<(32-i)) >> (32-1);
        if(current->child[val] == NULL){
            return NULL;
        }
        current = current->child[val];
    }
    return current;
}

int trie_delete(trie_t *trie, ip_t *ip) {
    node_t *current = trie->root;
    for(int i=0; i<ip->mask; ++i){
        int val = (ip->mask & 2<<(32-i)) >> (32-1);
        if(current->child[val] == NULL){
            return -1;
        }
        current = current->child[val];
    }
    if(current->ip){
        free(current->ip);
        current->ip = NULL;
    }
    else{
        return -1;
    }
    node_t * parent;
    while(!current->child[0] && !current->child[1]){
        parent = current->parent;
        free(current);
        current = parent;
    }
    return 0;
}
