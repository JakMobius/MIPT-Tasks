
#ifndef unit_tests_h
#define unit_tests_h

#include <string.h>
#include <stdlib.h>
#include "sorting.h"

#define CALL_TEST(method, args...) method(__LINE__, args)

typedef enum e_test_result e_test_result;

enum e_test_result {
    E_TEST_RESULT_ILLEGAL_INPUT,
    E_TEST_RESULT_WRONG_OUTPUT,
    E_TEST_RESULT_OUT_OF_MEMORY,
    E_TEST_RESULT_OK
};

extern const char* TEST_FAILURE_DESCRIPTIONS[];

/**
 * @brief Performs single test
 * @param [in] input Simulated program input
 * @param [in] expected_output Expected program output
 * @param [in] walk_direction Setting, which will be used when running the program
 * @param [in] sort_type Specified which function should be used for sorting
 * @returns Test result
 */
e_test_result perform_test(const char* input, const char* expected_output, e_walk_direction walk_direction, e_sort_type sort_type);

/**
 * @brief Performs single test and prints its result
 * @param [in] input Simulated program input
 * @param [in] expected_output Expected program output
 * @param [in] walk_direction Setting, which will be used when running the program
 * @param [in] sort_type Specified which function should be used for sorting
 */
void print_test(int line, const char* input, const char* expected_output, e_walk_direction walk_direction, e_sort_type sort_type);

#endif /* unit_tests_h */
