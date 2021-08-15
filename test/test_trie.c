#include "ip.h"
#include "mask.h"
#include "test_utils.h"
#include "trie.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unity.h>

trie_t *trie = NULL;

void setUp(void) {
    trie = trie_init();
}

void tearDown(void) {
    trie_deinit(trie);
    trie = NULL;
}

void test_node_insert_wrong_ip(void) {
    ip_t ip = {
        .base = 86,
        .mask = 24};
    node_t *node = trie_insert(trie, &ip);
    TEST_ASSERT_TRUE(node == NULL);
}

void test_node_insert_small(void) {
    ip_t ip = {
        .base = ip_to_int(128, 0, 0, 0),
        .mask = 2};
    node_t *node = trie_insert(trie, &ip);
    TEST_ASSERT_TRUE(check_node_ip_and_mask(node));
}

void test_node_insert(void) {
    ip_t ip = {
        .base = ip_to_int(10, 20, 0, 0),
        .mask = 16};
    node_t *node = trie_insert(trie, &ip);
    TEST_ASSERT_TRUE(check_node_ip_and_mask(node));
}

void test_node_insert_two_identical_nodes(void) {
    ip_t ip = {
        .base = ip_to_int(10, 20, 0, 0),
        .mask = 16};
    trie_insert(trie, &ip);
    node_t *node = trie_insert(trie, &ip);
    TEST_ASSERT(node == NULL);
}

void test_node_insert_same_ip_different_mask(void) {
    ip_t ip1 = {
        .base = ip_to_int(10, 20, 0, 0),
        .mask = 16};

    ip_t ip2 = {
        .base = ip_to_int(10, 20, 0, 0),
        .mask = 24};
    node_t *node1 = trie_insert(trie, &ip1);
    node_t *node2 = trie_insert(trie, &ip2);

    TEST_ASSERT(check_node_ip_and_mask(node1));
    TEST_ASSERT(check_node_ip_and_mask(node2));
    node_t *current = node2;
    for (unsigned int i = 0; i < 8; ++i) {
        TEST_ASSERT_NOT_NULL(current);
        current = current->parent;
    }
    TEST_ASSERT(current == node1);
}

void test_node_insert_same_mask_different_ip(void) {
    ip_t ip1 = {
        .base = ip_to_int(10, 20, 0, 0),
        .mask = 16};

    ip_t ip2 = {
        .base = ip_to_int(20, 30, 0, 0),
        .mask = 16};
    node_t *node1 = trie_insert(trie, &ip1);
    node_t *node2 = trie_insert(trie, &ip2);

    TEST_ASSERT(check_node_ip_and_mask(node1));
    TEST_ASSERT(check_node_ip_and_mask(node2));
}

void test_node_insert_0_0(void) {
    ip_t ip1 = {
        .base = 0,
        .mask = 0};
    node_t *node1 = trie_insert(trie, &ip1);
    TEST_ASSERT(check_node_ip_and_mask(node1));
}

void test_node_insert_mask_32(void) {
    ip_t ip1 = {
        .base = ip_to_int(128, 65, 30, 23),
        .mask = 32};
    node_t *node1 = trie_insert(trie, &ip1);
    TEST_ASSERT(check_node_ip_and_mask(node1));
}

/* Create trie node and check it */
void test_trie_insert(void) {
    srand(5);
    for (int i = 0; i < 1024; ++i) {
        ip_t ip;
        ip.mask = rand() % IP_LENGTH + 1;
        ip.base = ((unsigned int) rand() * 2) & get_bitmask(ip.mask);
        node_t *node = trie_insert(trie, &ip);
        if (node == NULL) {
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
        ip_t ip;
        ip.mask = rand() % IP_LENGTH + 1;
        ip.base = ((unsigned int) rand() * 2) & get_bitmask(ip.mask);
        nodes[i] = trie_insert(trie, &ip);
        if (nodes[i] == NULL) --i;
        else
            ips[i] = nodes[i]->ip;
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
    ip_t ip;
    ip.base = ip_to_int(10, 10, 0, 0);
    ip.mask = 16;
    trie_insert(trie, &ip);
    TEST_ASSERT(trie_delete(trie, &ip) == 0);
    TEST_ASSERT_TRUE(trie->root == NULL);
}

void test_del_ip_wrong_address(void) {
    ip_t ip;
    ip.base = ip_to_int(10, 10, 0, 0);
    ip.mask = 16;
    trie_insert(trie, &ip);
    TEST_ASSERT(del(ip_to_int(10, 20, 0, 0), 16) == -1);
}

void test_del_create_entire_trie_and_delete_one(void) {
    unsigned int ip_number_to_del = 4;
    ip_t ips[] = {
        {ip_to_int(40, 0, 0, 0), 8},
        {ip_to_int(10, 0, 0, 0), 8},
        {ip_to_int(11, 0, 0, 0), 8},
        {ip_to_int(145, 0, 0, 0), 8},

        {ip_to_int(10, 10, 0, 0), 16},
        {ip_to_int(10, 11, 0, 0), 16},
        {ip_to_int(11, 11, 0, 0), 16},
        {ip_to_int(145, 10, 0, 0), 16},

        {ip_to_int(10, 10, 1, 0), 24},
        {ip_to_int(10, 11, 2, 0), 24},
        {ip_to_int(11, 11, 1, 0), 24},
        {ip_to_int(145, 10, 30, 0), 24},
    };

    node_t *nodes[12] = {};

    for (int i = 0; i < sizeof(ips) / sizeof(ips[0]); ++i) {
        nodes[i] = trie_insert(trie, &ips[i]);
    }
    TEST_ASSERT(trie_delete(trie, &ips[ip_number_to_del]) == 0);

    char error_msg[256];
    for (int i = 0; i < sizeof(ips) / sizeof(ips[0]); ++i) {
        if (i == ip_number_to_del) continue;
        sprintf(error_msg, "Error at element %d", i);
        TEST_ASSERT_MESSAGE(check_node_ip_and_mask(nodes[i]), error_msg);
        TEST_ASSERT(nodes[i]->ip->base == ips[i].base);
        TEST_ASSERT(nodes[i]->ip->mask == ips[i].mask);
    }

    TEST_ASSERT(del(ip_to_int(10, 20, 0, 0), 16) == -1);
}

void test_del_create_entire_trie_and_delete_all(void) {
    ip_t ips[] = {
        {ip_to_int(40, 0, 0, 0), 8},
        {ip_to_int(10, 0, 0, 0), 8},
        {ip_to_int(11, 0, 0, 0), 8},
        {ip_to_int(145, 0, 0, 0), 8},

        {ip_to_int(10, 10, 0, 0), 16},
        {ip_to_int(10, 11, 0, 0), 16},
        {ip_to_int(11, 11, 0, 0), 16},
        {ip_to_int(145, 10, 0, 0), 16},

        {ip_to_int(10, 10, 1, 0), 24},
        {ip_to_int(10, 11, 2, 0), 24},
        {ip_to_int(11, 11, 1, 0), 24},
        {ip_to_int(145, 10, 30, 0), 24},
    };

    node_t *nodes[12] = {};

    char error_msg[256];
    for (int i = 0; i < sizeof(ips) / sizeof(ips[0]); ++i) {
        nodes[i] = trie_insert(trie, &ips[i]);
    }

    for (int i = 0; i < sizeof(ips) / sizeof(ips[0]); ++i) {
        sprintf(error_msg, "Error at element %d", i);
        TEST_ASSERT_MESSAGE(trie_delete(trie, &ips[i]) == 0, error_msg);
    }
    TEST_ASSERT(trie->root == NULL);
}

void test_del_0_0(void) {
    ip_t ip1 = {
        .base = 0,
        .mask = 0};
    trie_insert(trie, &ip1);
    TEST_ASSERT(trie_delete(trie, &ip1) == 0);
}

void test_trie_check(void) {
    ip_t ip;
    ip.base = ip_to_int(10, 10, 0, 0);
    ip.mask = 16;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(10, 20, 1, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(10, 30, 1, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);

    node_t *node = trie_check(trie, ip_to_int(10, 10, 20, 1));
    TEST_ASSERT(node->ip->base == ip_to_int(10, 10, 0, 0));
    TEST_ASSERT(node->ip->mask == 16);
}

void test_check_wrong_ip(void) {
    ip_t ip;
    ip.base = ip_to_int(10, 10, 0, 0);
    ip.mask = 16;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(10, 20, 1, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(10, 30, 1, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(10, 0, 0, 0);
    ip.mask = 8;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(100, 40, 32, 1);
    ip.mask = 32;
    trie_insert(trie, &ip);

    node_t *node = trie_check(trie, ip_to_int(50, 10, 20, 1));
    TEST_ASSERT(node == NULL);
}

void test_check_maximum_mask(void) {
    ip_t ip;
    ip.base = ip_to_int(10, 10, 0, 0);
    ip.mask = 16;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(10, 20, 1, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(10, 30, 1, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(50, 40, 32, 1);
    ip.mask = 32;
    trie_insert(trie, &ip);

    ip.base = ip_to_int(100, 0, 0, 0);
    ip.mask = 8;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(100, 40, 0, 0);
    ip.mask = 16;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(100, 40, 32, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(100, 40, 32, 1);
    ip.mask = 32;
    trie_insert(trie, &ip);

    node_t *node = trie_check(trie, ip_to_int(100, 50, 50, 50));
    TEST_ASSERT(node->ip->base == ip_to_int(100, 0, 0, 0) && node->ip->mask == 8);
    node = trie_check(trie, ip_to_int(100, 40, 50, 50));
    TEST_ASSERT(node->ip->base == ip_to_int(100, 40, 0, 0) && node->ip->mask == 16);
    node = trie_check(trie, ip_to_int(100, 40, 32, 50));
    TEST_ASSERT(node->ip->base == ip_to_int(100, 40, 32, 0) && node->ip->mask == 24);
    node = trie_check(trie, ip_to_int(100, 40, 32, 1));
    TEST_ASSERT(node->ip->base == ip_to_int(100, 40, 32, 1) && node->ip->mask == 32);
}

void test_check_same_ip_diff_mask(void) {
    ip_t ip;
    ip.base = ip_to_int(10, 10, 0, 0);
    ip.mask = 16;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(10, 20, 1, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(10, 30, 1, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(50, 40, 32, 1);
    ip.mask = 32;
    trie_insert(trie, &ip);

    ip.base = ip_to_int(100, 0, 0, 0);
    ip.mask = 8;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(100, 0, 0, 0);
    ip.mask = 16;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(100, 0, 0, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);

    node_t *node = trie_check(trie, ip_to_int(100, 0, 0, 0));
    TEST_ASSERT(node->ip->base == ip_to_int(100, 0, 0, 0) && node->ip->mask == 24);
}

void test_check_ip_0_0(void) {
    ip_t ip;
    ip.base = ip_to_int(10, 10, 0, 0);
    ip.mask = 16;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(10, 20, 1, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(10, 30, 1, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(50, 40, 32, 1);
    ip.mask = 32;
    trie_insert(trie, &ip);

    node_t *node = trie_check(trie, ip_to_int(0, 0, 0, 0));
    TEST_ASSERT(node == NULL);

    ip.base = ip_to_int(0, 0, 0, 0);
    ip.mask = 0;
    trie_insert(trie, &ip);
    node = trie_check(trie, ip_to_int(0, 0, 0, 0));
    TEST_ASSERT(node->ip->base == ip_to_int(0, 0, 0, 0) && node->ip->mask == 0);
}

void test_search(void) {
    ip_t ip;

    ip.base = ip_to_int(243, 20, 0, 0);
    ip.mask = 16;
    trie_insert(trie, &ip);

    ip.base = ip_to_int(243, 20, 32, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);

    ip.base = ip_to_int(10, 10, 0, 0);
    ip.mask = 16;
    node_t *node = trie_insert(trie, &ip);
    node_t *node_searched = trie_search(trie, &ip);
    TEST_ASSERT(node == node_searched);
}

void test_search_wrong(void) {
    ip_t ip_to_search = {
        .base = ip_to_int(10, 10, 0, 0),
        .mask = 16};
    ip_t ip;
    ip.base = ip_to_int(243, 20, 0, 0);
    ip.mask = 16;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(243, 20, 32, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(128, 20, 32, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(25, 20, 32, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);
    ip.base = ip_to_int(69, 69, 69, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);
    node_t *node_searched = trie_search(trie, &ip_to_search);
    TEST_ASSERT(node_searched == NULL);
}

void test_search_0_0(void) {
    ip_t ip;

    ip.base = ip_to_int(243, 20, 0, 0);
    ip.mask = 16;
    trie_insert(trie, &ip);

    ip.base = ip_to_int(243, 20, 0, 0);
    ip.mask = 16;
    trie_insert(trie, &ip);

    ip.base = ip_to_int(243, 20, 32, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);

    ip.base = ip_to_int(128, 20, 32, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);

    ip.base = ip_to_int(25, 20, 32, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);

    ip.base = ip_to_int(49, 49, 49, 0);
    ip.mask = 24;
    trie_insert(trie, &ip);

    ip.base = ip_to_int(0, 0, 0, 0);
    ip.mask = 0;
    node_t *node = trie_insert(trie, &ip);
    node_t *node_searched = trie_search(trie, &ip);
    TEST_ASSERT(node_searched == node);
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_node_insert);
    RUN_TEST(test_node_insert_small);
    RUN_TEST(test_node_insert_wrong_ip);
    RUN_TEST(test_node_insert_two_identical_nodes);
    RUN_TEST(test_node_insert_same_ip_different_mask);
    RUN_TEST(test_node_insert_same_mask_different_ip);
    RUN_TEST(test_node_insert_0_0);
    RUN_TEST(test_node_insert_mask_32);
    RUN_TEST(test_trie_insert);
    RUN_TEST(test_trie_insert_all);

    RUN_TEST(test_del_ip);
    RUN_TEST(test_del_ip_wrong_address);
    RUN_TEST(test_del_create_entire_trie_and_delete_one);
    RUN_TEST(test_del_create_entire_trie_and_delete_all);
    RUN_TEST(test_del_0_0);

    RUN_TEST(test_trie_check);
    RUN_TEST(test_check_wrong_ip);
    RUN_TEST(test_check_maximum_mask);
    RUN_TEST(test_check_same_ip_diff_mask);
    RUN_TEST(test_check_ip_0_0);
    RUN_TEST(test_search);
    RUN_TEST(test_search_wrong);
    RUN_TEST(test_search_0_0);

    return UNITY_END();
}