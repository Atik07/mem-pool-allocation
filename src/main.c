#include "mavalloc.c"
#include "tinytest.h"
#include <stdio.h>

#define DEBUG 0

int test_case_1()
{
    mavalloc_init(69369, BEST_FIT);
    if (DEBUG)
        print_list(root);

    char *ptr = (char *)mavalloc_alloc(65535);
    if (DEBUG)
        print_list(root);

    mavalloc_free(ptr);
    if (DEBUG)
        print_list(root);

    return 1;
}

int test_case_2()
{
    mavalloc_init(2048, FIRST_FIT);
    if (DEBUG)
        print_list(root);

    char *ptr1 = (char *)mavalloc_alloc(2048);
    if (DEBUG)
        print_list(root);

    mavalloc_free(ptr1);
    if (DEBUG)
        print_list(root);

    char *ptr2 = (char *)mavalloc_alloc(1024);
    if (DEBUG)
        print_list(root);

    mavalloc_free(ptr2);
    if (DEBUG)
        print_list(root);

    char *ptr4 = (char *)mavalloc_alloc(96);
    if (DEBUG)
        print_list(root);

    char *ptr5 = (char *)mavalloc_alloc(452);
    if (DEBUG)
        print_list(root);

    mavalloc_free(ptr4);
    if (DEBUG)
        print_list(root);

    char *ptr3 = (char *)mavalloc_alloc(48);
    if (DEBUG)
        print_list(root);

    char *ptr6 = (char *)mavalloc_alloc(678);
    if (DEBUG)
        print_list(root);

    mavalloc_free(ptr5);
    if (DEBUG)
        print_list(root);

    char *ptr7 = (char *)mavalloc_alloc(312);
    if (DEBUG)
        print_list(root);

    mavalloc_destroy();
    if (DEBUG)
        print_list(root);

    return 1;
}

int test_case_3()
{
    // intentionally fail this test for now

    TINYTEST_EQUAL(1, 0);

    return 1;
}

int tinytest_setup(const char *pName)
{
    fprintf(stderr, "tinytest_setup(%s)\n", pName);
    return 0;
}

int tinytest_teardown(const char *pName)
{
    fprintf(stderr, "tinytest_teardown(%s)\n", pName);
    return 0;
}

TINYTEST_START_SUITE(MavAllocTestSuite);
TINYTEST_ADD_TEST(test_case_1, tinytest_setup, tinytest_teardown);
TINYTEST_ADD_TEST(test_case_2, tinytest_setup, tinytest_teardown);
TINYTEST_ADD_TEST(test_case_3, tinytest_setup, tinytest_teardown);
TINYTEST_END_SUITE();

TINYTEST_MAIN_SINGLE_SUITE(MavAllocTestSuite);
