
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
    E_TEST_RESULT_OK
};

extern const char* TEST_FAILURE_DESCRIPTIONS[];

/**
 * @brief Performs single test
 * @param [in] input Simulated program input
 * @param [in] expected_output Expected program output
 * @param [in] mode Setting, which will be used when running the program
 * @returns Test result
 */
e_test_result perform_test(const char* input, const char* expected_output, e_walk_direction mode);

/**
 * @brief Performs single test and prints its result
 * @param [in] input Simulated program input
 * @param [in] expected_output Expected program output
 * @param [in] mode Setting, which will be used when running the program
 */
void print_test(int line, const char* input, const char* expected_output, e_walk_direction mode);

#endif /* unit_tests_h */
