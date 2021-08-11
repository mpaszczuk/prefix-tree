#include "ip.h"
#include <stdlib.h>

static trie_t *trie;

void init_ip_trie(trie_t *trie_) {
    trie = trie_;
}

int add(unsigned int base, char mask) {
    ip_t ip = {
        .base = base,
        .mask = mask
    };
    ip_t *node_ip;
    node_t * node = trie_search(trie, &ip);
    if(node != NULL && node->ip != NULL) {
        return -1;
    }
    node_ip = (ip_t *) malloc(sizeof(ip_t));
    node_ip->base = base;
    node_ip->mask = mask;
    trie_insert(trie, node_ip);
    return 0;
}
int del(unsigned int base, char mask) {
    ip_t ip = {
        .base = base,
        .mask = mask
    };
    return trie_delete(trie, &ip);
}
char check(unsigned int ip) {
    node_t *ip_node = trie_check(trie, ip);
    if (ip_node != NULL) {
        return ip_node->ip->mask;
    }
    return -1;
}

