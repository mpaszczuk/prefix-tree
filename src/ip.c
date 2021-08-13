#include "ip.h"
#include <stdlib.h>

static trie_t *trie;

void init_ip_trie(trie_t *trie_) {
    trie = trie_;
    trie->root = NULL;
}

void deinit_ip_trie() {
    trie_deinit(trie);
}

int add(unsigned int base, char mask) {
    ip_t ip = {
        .base = base,
        .mask = mask};
    trie_insert(trie, &ip);
    return 0;
}
int del(unsigned int base, char mask) {
    ip_t ip = {
        .base = base,
        .mask = mask};
    return trie_delete(trie, &ip);
}
char check(unsigned int ip) {
    node_t *ip_node = trie_check(trie, ip);
    if (ip_node != NULL) {
        return ip_node->ip->mask;
    }
    return -1;
}
