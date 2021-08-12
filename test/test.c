#include "ip.h"
#include "malloc.h"
#include "stdbool.h"
#include <stdlib.h>
#include "trie.h"
#include "unity.h"

trie_t *trie = NULL;

void setUp(void) {
    trie = malloc(sizeof(trie_t));
    init_ip_trie(trie);
}

void tearDown(void) {
    deinit_ip_trie();
    trie = NULL;
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
    node_t * current = node;
    for (mask = 0; mask < IP_LENGTH; ++mask) {
        if (current->parent != NULL) {
            unsigned int tmp = current->parent->child[LEFT_CHILD] == current ? LEFT_CHILD : RIGHT_CHILD;
            ip |= tmp << mask;
            current = current->parent;
        } else {
            break;
        }
    }
    ip = ip <<(IP_LENGTH - mask);
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

void test_trie_insert(void ){
    int i = RAND_MAX;
    srand(5);
    for(int i = 0; i<1024; ++i){
        ip_t *ip = malloc(sizeof(ip_t));
        ip->mask = rand() % IP_LENGTH + 1;
        ip->base = ((unsigned int)rand() * 2) ;
        node_t *node = trie_insert(trie, ip);
        TEST_ASSERT_TRUE(check_node_ip_and_mask(node));
    }

}

void test_del_ip(void) {
    add(0b11 << 30, 4);
    add(1 << 31, 4);
    add(1510, 4);
    add(1023, 3);
    add(102310, 5);
    add(40, 2);
    add(151011, 4);
    trie_print(trie->root, 0);
    del(40, 2);
    trie_print(trie->root, 0);
}

void test_fail(void) {
    TEST_ASSERT_TRUE(0);
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
//    RUN_TEST(test_node_insert);
    RUN_TEST(test_trie_insert);
    return UNITY_END();
}