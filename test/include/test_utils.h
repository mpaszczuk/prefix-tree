#pragma once
#include "trie.h"
#include <stdbool.h>

void trie_print(node_t *root, int space);
unsigned int ip_to_int(unsigned char ip1, unsigned char ip2, unsigned char ip3, unsigned char ip4);
bool check_parent_child_relation(node_t *parent, node_t *child, int child_number);
bool check_node_ip_and_mask(node_t *node);
