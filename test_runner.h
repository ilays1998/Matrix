#ifndef _TEST_RUNNER_H_
#define _TEST_RUNNER_H_

#include <stdlib.h>

typedef int (*FuncPointer) ();

typedef struct TestInfo {
    char *description;
    FuncPointer func_ptr;
} TestInfo;

typedef struct TestSuite {
    TestInfo *tests;
    size_t amount_of_tests;
    FuncPointer setup;
    FuncPointer teardown;
} TestSuite;


void run_tests (TestSuite test_suite);

#endif //_TEST_RUNNER_H_
