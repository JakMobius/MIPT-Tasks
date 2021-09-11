
#ifndef decompiler_h
#define decompiler_h

#include <stdbool.h>
#include <stdio.h>

#include "../shared/registers.h"
#include "../shared/commands.h"
#include "register_names.h"
#include "command_names.h"

typedef enum decompiler_state {
    DECOMPILER_STATE_RUNNING,
    DECOMPILER_STATE_ERROR,
    DECOMPILER_STATE_REACHED_FILE_END
} s_decompiler_state;

typedef struct decompiler_config {
    const char* input;
    unsigned long long input_length;
    FILE* output_file;
    bool ignore_line_flags;
} s_decompiler_config;

typedef struct decompiler {
    const char* input;
    unsigned long long input_length;
    unsigned long long position;
    s_decompiler_state state;
    s_decompiler_config* config;
} s_decompiler;

unsigned char decompiler_char_ahead(s_decompiler* thou);

void decompiler_print_stack_argument(s_decompiler* thou, e_proc_flags flag);

void decompiler_pipe_number(s_decompiler* thou, e_proc_flags flag);

void decompiler_print_number(s_decompiler* thou, e_proc_flags flag, unsigned long long buffer);

unsigned long long decompiler_read_integer(s_decompiler* thou, size_t bytes);

void decompiler_previous_char(s_decompiler* thou);

unsigned char decompiler_next_char(s_decompiler* thou);

bool decompiler_perform(s_decompiler_config* config);

#endif /* decompiler_h */
