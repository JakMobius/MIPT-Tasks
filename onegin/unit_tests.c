
#include "unit_tests.h"

const char* TEST_FAILURE_DESCRIPTIONS[] = {
    "Illegal test input",
    "Program result didn't match expected",
    "Test passed"
};

e_test_result perform_test(const char* input, const char* expected_output, e_walk_direction mode) {
    
    if(input == NULL || expected_output == NULL) {
        return E_TEST_RESULT_ILLEGAL_INPUT;
    }
    
    size_t input_length = strlen(input);
    
    if(strlen(expected_output) != input_length) {
        return E_TEST_RESULT_ILLEGAL_INPUT;
    }
    
    char* input_copy = malloc(input_length * sizeof(char));
    
    CHECK_POINTER(input_copy);
    
    strcpy(input_copy, input);
    
    size_t line_num = replace_chars(input_copy, '\n', '\0');
    char* out_buffer = malloc(input_length * sizeof(char));
    
    sort_buffer(input_copy, line_num, input_length, out_buffer, mode);
    
    if(strcmp(out_buffer, expected_output) == 0) {
        return E_TEST_RESULT_OK;
    } else {
        return E_TEST_RESULT_WRONG_OUTPUT;
    }
}

void print_test(int line, const char* input, const char* expected_output, e_walk_direction mode) {
    e_test_result test_result = perform_test(input, expected_output, mode);
    
    if(test_result != E_TEST_RESULT_OK) {
        printf("Test failed on line %d: %s\n", line, TEST_FAILURE_DESCRIPTIONS[test_result]);
    } else {
        printf("Test passed\n");
    }
}
