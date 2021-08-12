#pragma once

#define LEFT_CHILD 0
#define RIGHT_CHILD 1
#define MAX_NUMBER_OF_CHILDREN 2

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

typedef struct {
    node_t *root;
} trie_t;

node_t *new_node_t();
void trie_init(trie_t *trie_);
void trie_deinit(trie_t *trie);
node_t *trie_insert(trie_t *trie, ip_t *ip);
node_t *trie_search(trie_t *trie, ip_t *ip);
int trie_delete(trie_t *trie, ip_t *ip);
node_t *trie_check(trie_t *trie, unsigned int ip);
void trie_print(node_t *root, int space);
