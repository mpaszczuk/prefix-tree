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

unsigned int ip_to_int(unsigned char ip1, unsigned char ip2, unsigned char ip3, unsigned char ip4) {
    return (unsigned int) (ip1 << 24) | (ip2 << 16) | (ip3 << 8) | ip4;
}

bool check_parent_child_relation(node_t *parent, node_t *child, int child_number) {
    /* Check parent-child and child-parent relation */
    if (child->parent != parent || parent->child[child_number] != child) {
        return false;
    }
    return true;
}

bool check_node_ip_and_mask(node_t *node) {
    if(node == NULL){
        return false;
    }
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

void test_node_insert_wrong_ip(void) {
    ip_t ip = {
        .base = 86,
        .mask = 32
    };
    node_t *node = trie_insert(trie, &ip);
    TEST_ASSERT_TRUE(node == NULL);
}

void test_node_insert(void) {
    ip_t ip = {
        .base = ip_to_int(10,20,0,0),
        .mask = 16
    };
    node_t *node = trie_insert(trie, &ip);
    TEST_ASSERT_TRUE(check_node_ip_and_mask(node));
}

void test_node_insert_two_identical_nodes(void) {
    ip_t ip = {
        .base = ip_to_int(10,20,0,0),
        .mask = 16
    };
    trie_insert(trie, &ip);
    node_t *node = trie_insert(trie, &ip);
    TEST_ASSERT(node == NULL);
}

void test_node_insert_same_ip_different_mask(void) {
    ip_t ip1 = {
        .base = ip_to_int(10,20,0,0),
        .mask = 16
    };

    ip_t ip2 = {
        .base = ip_to_int(10,20,0,0),
        .mask = 24
    };
    node_t *node1 = trie_insert(trie, &ip1);
    node_t *node2 = trie_insert(trie, &ip2);

    TEST_ASSERT(check_node_ip_and_mask(node1));
    TEST_ASSERT(check_node_ip_and_mask(node2));
    node_t *current = node2;
    for(unsigned int i =0; i<8; ++i){
        TEST_ASSERT_NOT_NULL(current);
        current = current->parent;
    }
    TEST_ASSERT(current == node1);
}

void test_node_insert_same_mask_different_ip(void) {
    ip_t ip1 = {
        .base = ip_to_int(10,20,0,0),
        .mask = 16
    };

    ip_t ip2 = {
        .base = ip_to_int(20,30,0,0),
        .mask = 16
    };
    node_t *node1 = trie_insert(trie, &ip1);
    node_t *node2 = trie_insert(trie, &ip2);

    TEST_ASSERT(check_node_ip_and_mask(node1));
    TEST_ASSERT(check_node_ip_and_mask(node2));
}

void test_node_insert_0_0(void) {
    ip_t ip1 = {
        .base = 0,
        .mask = 0
    };
    node_t *node1 = trie_insert(trie, &ip1);
    TEST_ASSERT(check_node_ip_and_mask(node1));
}

/* Create trie node and check it */
void test_trie_insert(void) {
    srand(5);
    for (int i = 0; i < 1024; ++i) {
        ip_t ip = {
            .mask = rand() % IP_LENGTH + 1,
            .base = ((unsigned int) rand() * 2) & get_bitmask(ip.mask)
        };
        node_t *node = trie_insert(trie, &ip);
        if(node == NULL){
            continue;
        }
        bool test_result = check_node_ip_and_mask(node);
        TEST_ASSERT_TRUE(test_result);
    }
}

/* Create entire trie and check every node */
void test_trie_insert_all(void) {
    const int num_of_tests = 1024;
    ip_t *ips[num_of_tests];
    node_t *nodes[num_of_tests];
    srand(5);
    for (int i = 0; i < num_of_tests; ++i) {
        ip_t ip = {
            .mask = rand() % IP_LENGTH + 1,
            .base = ((unsigned int) rand() * 2) & get_bitmask(ip.mask)
        };
        nodes[i] = trie_insert(trie, &ip);
        if(nodes[i] ==NULL) --i;
        else ips[i] = nodes[i]->ip;
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

void test_del_ip_from_trie(void) {
    add(ip_to_int(10,10,0,0), 16);
    add(ip_to_int(10,20,3,0), 24);
    add(ip_to_int(10,10,40,20), 32);
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
    RUN_TEST(test_node_insert);
    RUN_TEST(test_node_insert_wrong_ip);
    RUN_TEST(test_node_insert_two_identical_nodes);
    RUN_TEST(test_node_insert_same_ip_different_mask);
    RUN_TEST(test_node_insert_same_mask_different_ip);
    RUN_TEST(test_node_insert_0_0);
    RUN_TEST(test_trie_insert);
    RUN_TEST(test_trie_insert_all);
    RUN_TEST(test_del_ip);
//    RUN_TEST(test_search);
//    RUN_TEST(test_check);

    return UNITY_END();
}