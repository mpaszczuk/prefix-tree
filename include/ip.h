#pragma once

typedef struct {
    unsigned int base;
    char mask;
} ip_t;

int add(unsigned int base, char mask);
int del(unsigned int base, char mask);
char check(unsigned int ip);
int compare(const ip_t *ip1, const ip_t *ip2);
