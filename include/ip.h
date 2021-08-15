#pragma once

#define IP_LENGTH 32
#define IP_ERROR (-1)
#define IP_OK 0

void init_ip_trie();
void deinit_ip_trie();
int add(unsigned int base, char mask);
int del(unsigned int base, char mask);
char check(unsigned int ip);
