#include "unity.h"
#include "trie.h"
#include "ip.h"
#include "malloc.h"

trie_t *trie = NULL;

void setUp(void) {
    trie = malloc(sizeof(trie_t));
    init_ip_trie(trie);
}

void tearDown(void) {
    deinit_ip_trie();
}

void test_function_should_doBlahAndBlah(void) {
    //test stuff
}

void test_add_ip(void) {
    add(11<<30, 4);
    add(1<<31, 4);
    add(1510, 4);
    add(1023, 3);
    add(102310, 5);
    add(40, 2);
    add(151011, 4);
    trie_print(trie->root, 0);
}

void test_fail(void) {
    TEST_ASSERT_TRUE(0);
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
//    RUN_TEST(test_function_should_doBlahAndBlah);
    RUN_TEST(test_add_ip);
//    RUN_TEST(test_fail);
    return UNITY_END();
}