#include "trie.h"
#include "mask.h"
#include <stddef.h>
#include <stdlib.h>

static node_t *new_node_t();
static ip_t *new_ip_t();
static int del_node_t(node_t *node);

node_t *new_node_t() {
    node_t *node = malloc(sizeof(node_t));
    node->child = malloc(MAX_NUMBER_OF_CHILDREN * sizeof(node_t *));
    node->child[LEFT_CHILD] = node->child[RIGHT_CHILD] = NULL;
    node->ip = NULL;
    node->parent = NULL;
    return node;
}

ip_t *new_ip_t() {
    ip_t *ip = (ip_t *) malloc(sizeof(ip_t));
    ip->base = 0;
    ip->mask = 0;
    return ip;
}

int del_node_t(node_t *node) {
    if (node == NULL) {
        return TRIE_ERROR;
    }
    if (node->ip != NULL) {
        free(node->ip);
        node->ip = NULL;
    }
    if (node->child != NULL) {
        if (node->child[LEFT_CHILD] != NULL) {
            free(node->child[LEFT_CHILD]);
        }
        if (node->child[RIGHT_CHILD] != NULL) {
            free(node->child[RIGHT_CHILD]);
        }
        free(node->child);
        node->child = NULL;
    }
    if (node->parent != NULL) {
        if (node->parent->child[LEFT_CHILD] == node) {
            node->parent->child[LEFT_CHILD] = NULL;
        } else if (node->parent->child[RIGHT_CHILD] == node) {
            node->parent->child[RIGHT_CHILD] = NULL;
        }
    }
    free(node);
    return TRIE_OK;
}

node_t *trie_insert(trie_t *trie, ip_t *ip_) {
    node_t *node = trie_search(trie, ip_);
    if (node != NULL && node->ip != NULL) {
        return NULL;
    }
    /* Check if ip base has the same number of bits as bitmask. In case of differences
     * function returns NULL ptr. Mask should be the same length as ip */
    if (ip_->base != (ip_->base & get_bitmask(ip_->mask))) {
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
    if (ip->mask == 0) {
        trie->root->ip = ip;
        return trie->root;
    }
    node_t *current = trie->root;
    /*Iterate through subsequent bits of ip  */
    for (unsigned int i = 0; i < ip->mask; ++i) {
        unsigned int bit = (ip->base & (1 << (UINT32_LEN - i - 1))) >> (UINT32_LEN - i - 1);
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
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < MAX_NUMBER_OF_CHILDREN; ++i) {
        if (node->child[i] != NULL) {
            node_deinit(node->child[i]);
            node->child[i] = NULL;
        }
    }
    del_node_t(node);
}

trie_t *trie_init() {
    trie_t *trie = (trie_t *) malloc(sizeof(trie_t));
    trie->root = NULL;
    return trie;
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
    /* Find the closest match node, even without set ip */
    for (unsigned int i = 0; current != NULL; ++i) {
        previous = current;
        unsigned int bit = get_bit(ip, i);
        current = current->child[bit];
    }
    /* Go back in branch and find node with ip */
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
            return TRIE_ERROR;
        }
        current = current->child[bit];
    }
    if (current->ip) {
        free(current->ip);
        current->ip = NULL;
    } else {
        return TRIE_ERROR;
    }
    node_t *parent;
    /* Remove nodes in branch without any child and ip. */
    while (current != NULL && current->ip == NULL && !current->child[LEFT_CHILD] && !current->child[RIGHT_CHILD]) {
        parent = current->parent;
        if (current == trie->root) {
            trie->root = NULL;
        }
        del_node_t(current);
        current = parent;
    }
    return TRIE_OK;
}
