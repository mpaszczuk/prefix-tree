#include <stdio.h>
#include <malloc.h>
#include "trie.h"

int main() {
    node_t *root   = new_node_t();
    root->ip->base = 1;
    root->child[0]  = new_node_t();
    root->child[0]->ip->base = 2;
    root->child[1] = new_node_t();
    root->child[1]->ip->base = 3;

    root->child[0]->child[0]  = new_node_t();
    root->child[0]->child[0] ->ip->base = 4;
    root->child[0]->child[1] = new_node_t();
    root->child[0]->child[1]->ip->base = 5;

    root->child[1]->child[0]  = new_node_t();
    root->child[1]->child[0] ->ip->base = 6;
    root->child[1]->child[1] = new_node_t();
    root->child[1]->child[1]->ip->base = 7;

    root->child[0]->child[0]->child[0]  = new_node_t();
    root->child[0]->child[0]->child[0] ->ip->base = 8;
    root->child[0]->child[0]->child[1]  = new_node_t();
    root->child[0]->child[0]->child[1] ->ip->base = 9;
    root->child[0]->child[1]->child[0]  = new_node_t();
    root->child[0]->child[1]->child[0] ->ip->base = 10;
    root->child[0]->child[1]->child[1]  = new_node_t();
    root->child[0]->child[1]->child[1] ->ip->base = 11;
    root->child[1]->child[0]->child[0]  = new_node_t();
    root->child[1]->child[0]->child[0] ->ip->base = 12;
    root->child[1]->child[0]->child[1]  = new_node_t();
    root->child[1]->child[0]->child[1] ->ip->base = 13;
    root->child[1]->child[1]->child[0]  = new_node_t();
    root->child[1]->child[1]->child[0] ->ip->base = 14;
    root->child[1]->child[1]->child[1]  = new_node_t();
    root->child[1]->child[1]->child[1] ->ip->base = 15;
    trie_print(root, 0);
    return 0;
}
