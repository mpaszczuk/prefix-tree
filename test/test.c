#include "ip.h"
#include "malloc.h"
#include "stdbool.h"
#include "trie.h"
#include "unity.h"
#include <stdlib.h>

trie_t *trie = NULL;

void setUp(void) {
    trie = malloc(sizeof(trie_t));
    init_ip_trie(trie);
}

void tearDown(void) {
    deinit_ip_trie();
    trie = NULL;
}

unsigned int ip_to_int(unsigned int ip1, unsigned int ip2, unsigned int ip3, unsigned int ip4) {
    return (ip1 << 24) | (ip2 << 16) | (ip3 << 8) | ip4;
}

bool check_parent_child_relation(node_t *parent, node_t *child, int child_number) {
    /* Check parent-child and child-parent relation */
    if (child->parent != parent || parent->child[child_number] != child) {
        return false;
    }
    return true;
}

bool check_node_ip_and_mask(node_t *node) {
    unsigned int ip = 0;
    char mask;
    node_t *current = node;
    for (mask = 0; mask < IP_LENGTH; ++mask) {
        if (current->parent != NULL) {
            unsigned int tmp = current->parent->child[LEFT_CHILD] == current ? LEFT_CHILD : RIGHT_CHILD;
            ip |= tmp << mask;
            current = current->parent;
        } else {
            break;
        }
    }
    ip = ip << (IP_LENGTH - mask);
    return node->ip->base == ip && node->ip->mask == mask;
}

void test_function_should_doBlahAndBlah(void) {
    //test stuff
}

void test_node_insert(void) {
    ip_t *ip = malloc(sizeof(ip_t));
    ip->base = 86;
    ip->mask = 32;
    node_t *node = trie_insert(trie, ip);
    TEST_ASSERT_TRUE(check_node_ip_and_mask(node));
}

void test_trie_insert(void) {
    int i = RAND_MAX;
    srand(5);
    for (int i = 0; i < 1024; ++i) {
        ip_t *ip = malloc(sizeof(ip_t));
        ip->mask = rand() % IP_LENGTH + 1;
        ip->base = ((unsigned int) rand() * 2);
        node_t *node = trie_insert(trie, ip);
        TEST_ASSERT_TRUE(check_node_ip_and_mask(node));
    }
}

void test_trie_insert_all(void) {
    int i = RAND_MAX;
    const int num_of_tests = 1024;
    ip_t *ips[num_of_tests];
    node_t *nodes[num_of_tests];
    srand(5);
    for (int i = 0; i < num_of_tests; ++i) {
        ips[i] = malloc(sizeof(ip_t));
        ips[i]->mask = rand() % IP_LENGTH + 1;
        ips[i]->base = ((unsigned int) rand() * 2);
        nodes[i] = trie_insert(trie, ips[i]);
    }
    for (int i = 0; i < num_of_tests; ++i) {
        TEST_ASSERT_TRUE(check_node_ip_and_mask(nodes[i]));
        if (nodes[i]->ip != ips[i]) {
            asm("nop");
        }
        //        TEST_ASSERT_TRUE(nodes[i]->ip == ips[i]);
    }
}

void test_del_ip(void) {
    add(0b11 << 30, 4);
    TEST_ASSERT(del(0b11 << 30, 4) == 0);
    TEST_ASSERT_TRUE(trie->root == NULL);
}

void test_search(void) {
    ip_t *ip = malloc(sizeof(ip_t));
    ip->base = 190;
    ip->mask = 32;
    node_t *node = trie_insert(trie, ip);
    add(0b11 << 30, 4);
    add(40, 32);
    node_t *node_searched = trie_search(trie, ip);
    TEST_ASSERT(node == node_searched);
}

void test_check(void) {
    add(ip_to_int(10,10,0,0), 16);
    add(ip_to_int(10,20,1,0), 24);
    add(ip_to_int(10,30,1,0), 24);

    node_t * node= trie_check(trie, ip_to_int(10,10,20,1));
    TEST_ASSERT(node->ip->base == ip_to_int(10,10,0,0));
    TEST_ASSERT(node->ip->mask == 16);
}

void test_fail(void) {
    TEST_ASSERT_TRUE(0);
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    //    RUN_TEST(test_node_insert);
    RUN_TEST(test_trie_insert);
    RUN_TEST(test_trie_insert_all);
    RUN_TEST(test_del_ip);
    RUN_TEST(test_search);
    RUN_TEST(test_check);

    return UNITY_END();
}