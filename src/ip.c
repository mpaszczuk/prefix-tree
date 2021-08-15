#include "ip.h"
#include <stdlib.h>
#include "trie.h"

static trie_t *trie = NULL;

void init_ip_trie() {
    trie = trie_init();
}

void deinit_ip_trie() {
    trie_deinit(trie);
    trie = NULL;
}

int add(unsigned int base, char mask) {
    if(mask < 0 || mask > 32){
        return IP_ERROR;
    }
    if (trie == NULL) {
        return IP_ERROR;
    }
    ip_t ip = {
        .base = base,
        .mask = mask};
    node_t *node = trie_insert(trie, &ip);
    if (node == NULL) {
        return IP_ERROR;
    }
    return IP_OK;
}

int del(unsigned int base, char mask) {
    if(mask < 0 || mask > 32){
        return IP_ERROR;
    }
    if (trie == NULL) {
        return IP_ERROR;
    }
    ip_t ip = {
        .base = base,
        .mask = mask};
    return trie_delete(trie, &ip);
}

char check(unsigned int ip) {
    if (trie == NULL) {
        return IP_ERROR;
    }
    node_t *ip_node = trie_check(trie, ip);
    if (ip_node != NULL) {
        return ip_node->ip->mask;
    }
    return IP_ERROR;
}
