#include <stdio.h>
#include "test_runner.h"

#define PASS_COLOR "\033[0;32m"
#define FAIL_COLOR "\033[0;31m"
#define PARTIAL_FAIL_COLOR "\e[0;33m"
#define DEFAULT_COLOR "\033[0m"

#define PASS_STR "PASSED"
#define FAIL_STR "FAILED"

void print_passing_str (int result);
void print_result (int passed_count, size_t amount_of_tests);
int run_test (TestInfo test_info, int number, size_t amount_of_tests);

void run_tests (TestSuite test_suite)
{
    printf("Running Tests:\n");
    printf("=============\n");
    int passed_count = 0;
    for (int i = 0; i < test_suite.amount_of_tests; i++)
        {
            if(test_suite.setup != NULL) test_suite.setup();
            if (run_test (test_suite.tests[i], i + 1, test_suite.amount_of_tests))
                {
                    passed_count++;
                }
            if(test_suite.teardown != NULL) test_suite.teardown();
        }

    print_result (passed_count, test_suite.amount_of_tests);
}

void set_color (char *color)
{
    printf ("%s", color);
}

void print_in_color (char *text, char *color)
{
    set_color (color);
    printf ("%s", text);
    set_color (DEFAULT_COLOR);
}

int run_test (TestInfo test_info, int number, size_t amount_of_tests)
{
    printf ("(%d\\%lu) %s - ", number, amount_of_tests, test_info.description);
    int result = test_info.func_ptr ();
    print_passing_str (result);
    printf ("\n");

    return result;
}

void print_result (int passed_count, size_t amount_of_tests)
{
    printf ("====================\n");
    set_color (
        passed_count == amount_of_tests ? PASS_COLOR : PARTIAL_FAIL_COLOR);
    printf ("%d\\%lu tests passed !\n", passed_count, amount_of_tests);
    set_color (DEFAULT_COLOR);
    printf ("====================\n");
}

void print_passing_str (int result)
{
    print_in_color (result ? PASS_STR : FAIL_STR,
                    result ? PASS_COLOR : FAIL_COLOR);
}
