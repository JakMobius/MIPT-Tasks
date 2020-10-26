
#ifndef number_parser_h
#define number_parser_h

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

typedef union compiler_number {
    unsigned long long unsigned_value;
    long long signed_value;
    double double_value;
} u_compiler_number;

u_compiler_number read_number(s_compiler* thou, bool is_integer);

void compiler_write_number(s_compiler* thou, e_proc_flags flag, u_compiler_number number);

void compiler_pipe_number(s_compiler* thou, e_proc_flags flag);

#endif /* number_parser_h */
