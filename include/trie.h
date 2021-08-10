#pragma once

typedef struct {
    unsigned int base;
    char mask;
} ip_t;

typedef struct node_t node_t;

struct node_t {
    ip_t *ip;
    node_t **child;
    node_t *parent;
};

typedef struct{
    node_t *root;
} trie_t;


void init_trie(trie_t *trie_);
int trie_insert(trie_t *trie, ip_t *ip);
node_t *trie_search(trie_t *trie, ip_t *ip);
int trie_delete(trie_t *trie, ip_t *ip);
