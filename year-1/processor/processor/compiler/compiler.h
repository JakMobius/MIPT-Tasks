#ifndef compiler_h
#define compiler_h

#include "../shared/commands.h"
#include "../shared/registers.h"
#include "compiler_config.h"
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern char compiler_braces_forward[256];
extern char compiler_braces_backward[256];

typedef enum compiler_state {
    COMPILER_STATE_RUNNING,
    COMPILER_STATE_ERROR,
    COMPILER_STATE_FATAL_ERROR
} s_compiler_state;

typedef struct compiler_command_position {
    unsigned long long command_byte_position;
    unsigned char command_byte_length;  // It cannot be greater than 11...
    unsigned long command_text_position;
    unsigned char command_text_length; // As well
    bool is_erroneous;
} s_compiler_command_position;

typedef struct compiler_position {
    int line;
    int ch;
    long index;
    const char* filename;
} s_compiler_position;

typedef struct compiler_label_usage_point {
    unsigned long long address;
    struct compiler_position position;
} s_compiler_label_usage_point;

typedef struct compiler_label {
    unsigned long long position;
    s_compiler_label_usage_point* usages;
    unsigned long usages_buffer_length;
    unsigned long usages_buffer_capacity;
    struct compiler_label* next;
    bool resolved;
    char label[];
} s_compiler_label;

typedef struct compiler_token {
    s_compiler_position begin;
    s_compiler_position end;
} s_compiler_token;

typedef struct compiler {
    const char* text;
    s_compiler_state state;
    s_compiler_position position;
    s_compiler_config* config;
    char* compiler_output_buffer;
    s_compiler_label* label_list;
    s_compiler_command_position* command_positions;
    unsigned long command_amount;
    bool line_warning_emitted;
    
    unsigned long compiler_output_buffer_length;
    unsigned long compiler_output_buffer_capacity;
    unsigned long command_positions_buffer_length;
} s_compiler;


s_compiler_label* compiler_find_label(s_compiler* thou, s_compiler_token* token);

void compiler_label_add_usage(s_compiler* thou, s_compiler_label* label);

s_compiler_label* compiler_create_label(s_compiler* thou, s_compiler_token* token);

void compiler_add_label(s_compiler* thou, s_compiler_label* label);

int compiler_find_enum_index(const char* name, const char** string_list);

void compiler_out_buffer_extend(s_compiler* thou);

void compiler_write_bytes(s_compiler* thou, const void* ptr, unsigned long bytes);

void compiler_write_byte(s_compiler* thou, char byte);

void compiler_write_string(s_compiler* thou, const char* string);

void compiler_previous_char(s_compiler* thou);

char compiler_next_char(s_compiler* thou);

void compiler_skip_whitespaces(s_compiler* thou);

void compiler_read_until(s_compiler* thou, char expected);

void compiler_read_token(s_compiler* thou, s_compiler_token* target);

void compiler_read_next_command(s_compiler* thou, s_compiler_token* token);

void compiler_put_line_flag(s_compiler* thou);

void compiler_print_listing(s_compiler* thou);

bool compiler_perform(s_compiler_config* config);

bool is_whitespace(char c);

bool is_digit_16(char c);

void compiler_previous_char(s_compiler* thou);

char compiler_next_char(s_compiler* thou);

char compiler_ahead_char(s_compiler* thou);

#endif /* compiler_h */
