#include "ip.h"
#include "test_utils.h"
#include <unity.h>

void setUp(void) {
    init_ip_trie();
}

void tearDown(void) {
    deinit_ip_trie();
}

void test_api_add() {
    TEST_ASSERT(add(ip_to_int(243, 20, 0, 0), 16) == 0);
}

void test_api_add_wrong_mask() {
    TEST_ASSERT(add(ip_to_int(243, 20, 0, 0), 33) == -1);
    TEST_ASSERT(add(ip_to_int(243, 20, 0, 0), -1) == -1);
}

void test_api_del() {
    add(ip_to_int(243, 20, 0, 0), 16);
    add(ip_to_int(243, 21, 0, 0), 16);
    add(ip_to_int(243, 22, 0, 0), 16);
    TEST_ASSERT(del(ip_to_int(243, 20, 0, 0), 16) == 0);
}

void test_api_del_wrong_ip() {
    add(ip_to_int(243, 20, 0, 0), 16);
    TEST_ASSERT(del(ip_to_int(244, 20, 0, 0), 16) == -1);
}

void test_api_del_wrong_mask() {
    add(ip_to_int(243, 20, 0, 0), 16);
    TEST_ASSERT(del(ip_to_int(243, 20, 0, 0), 18) == -1);
}

void test_api_del_mask_out_of_range() {
    add(ip_to_int(243, 20, 0, 0), 16);
    TEST_ASSERT(del(ip_to_int(243, 20, 0, 0), -1) == -1);
    TEST_ASSERT(del(ip_to_int(243, 20, 0, 0), 33) == -1);
}

void test_api_check() {
    add(ip_to_int(243, 20, 0, 0), 16);
    TEST_ASSERT(check(ip_to_int(243, 20, 1, 0)) == 16);
}

void test_api_check_wrong_ip() {
    add(ip_to_int(243, 20, 0, 0), 16);
    TEST_ASSERT(check(ip_to_int(243, 21, 2, 0)) == -1);
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_api_add);
    RUN_TEST(test_api_add_wrong_mask);
    RUN_TEST(test_api_del);
    RUN_TEST(test_api_del_mask_out_of_range);
    RUN_TEST(test_api_del_wrong_ip);
    RUN_TEST(test_api_del_wrong_mask);
    RUN_TEST(test_api_check);
    RUN_TEST(test_api_check_wrong_ip);

    return UNITY_END();
}
