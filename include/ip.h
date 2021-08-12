#pragma once
#include "trie.h"

#define IP_LENGTH 32

void init_ip_trie(trie_t *trie_);
void deinit_ip_trie();
int add(unsigned int base, char mask);
int del(unsigned int base, char mask);
char check(unsigned int ip);
int compare(const ip_t *ip1, const ip_t *ip2);
