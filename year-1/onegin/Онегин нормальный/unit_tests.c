
#include "unit_tests.h"

const char* TEST_FAILURE_DESCRIPTIONS[] = {
    "Illegal test input",
    "Program result didn't match expected",
    "Out of memory",
    "Test passed"
};

e_test_result perform_test(const char* input, const char* expected_output, e_walk_direction walk_direction, e_sort_type sort_type) {
    
    if(input == NULL || expected_output == NULL) {
        return E_TEST_RESULT_ILLEGAL_INPUT;
    }
    
    size_t input_length = strlen(input);
    
    if(strlen(expected_output) != input_length) {
        return E_TEST_RESULT_ILLEGAL_INPUT;
    }
    
    s_text* text = s_text_create(input, input_length);
    
    if(walk_direction == E_WALK_DIRECTION_LTR) {
        sort_buffer(text, sort_type, comparator_ltr);
    } else if(walk_direction == E_WALK_DIRECTION_RTL) {
        sort_buffer(text, sort_type, comparator_rtl);
    } else {
        s_text_destroy(text);
        return E_TEST_RESULT_ILLEGAL_INPUT;
    }
    
    char* out_buffer = (char*)malloc(input_length + 1);
    
    s_text_compile(text, out_buffer);
    
    s_text_destroy(text);
    
    if(strcmp(out_buffer, expected_output) == 0) {
        return E_TEST_RESULT_OK;
    } else {
        return E_TEST_RESULT_WRONG_OUTPUT;
    }
}

void print_test(int line, const char* input, const char* expected_output, e_walk_direction walk_direction, e_sort_type sort_type) {
    e_test_result test_result = perform_test(input, expected_output, walk_direction, sort_type);
    
    if(test_result != E_TEST_RESULT_OK) {
        printf("Test failed on line %d: %s\n", line, TEST_FAILURE_DESCRIPTIONS[test_result]);
    } else {
        printf("Test passed\n");
    }
}
