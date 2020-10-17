#ifndef compiler_h
#define compiler_h

#include "../shared/commands.h"
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum compiler_state {
    COMPILER_STATE_RUNNING,
    COMPILER_STATE_ERROR
} compiler_state;

typedef struct compiler_position {
    long line;
    long ch;
    long index;
    const char* filename;
} compiler_position;

typedef struct compiler_token {
    compiler_position begin;
    compiler_position end;
} compiler_token;

typedef struct compiler {
    const char* text;
    compiler_state state;
    compiler_position position;
    FILE* output_stream;
} s_compiler;

int compiler_find_enum_index(const char* name, const char** string_list);

void compiler_pipe_number(s_compiler* thou, e_proc_flags flag);

void compiler_previous_char(s_compiler* thou);

char compiler_next_char(s_compiler* thou);

void compiler_skip_whitespaces(s_compiler* thou);

void compiler_read_until(s_compiler* thou, char expected);

void compiler_read_token(s_compiler* thou, compiler_token* target);

void read_command(s_compiler* thou);

bool compile(const char* text, const char* filename, FILE* output_stream);

bool is_whitespace(char c);

bool is_digit_16(char c);

void compiler_previous_char(s_compiler* thou);

char compiler_next_char(s_compiler* thou);

char compiler_ahead_char(s_compiler* thou);

#endif /* compiler_h */
