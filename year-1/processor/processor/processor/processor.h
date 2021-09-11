
#ifndef processor_h
#define processor_h

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "../shared/registers.h"
#include "videomemory.h"

typedef enum proc_state {
    PROC_STATE_RUNNING,
    PROC_STATE_HALTED,
    PROC_STATE_ERROR,
    PROC_STATE_REACHED_FILE_END,
    PROC_STATE_INVALID_OPCODE,
    PROC_STATE_OUT_OF_MEMORY,
    PROC_STATE_STACK_UNDERFLOW,
    PROC_STATE_DIVISION_BY_ZERO,
    PROC_STATE_MATHEMATICAL_EXCEPTION,
    PROC_ERROR_WINDOW_ALREADY_OPEN,
    PROC_ERROR_NO_WINDOW
} proc_state;

typedef struct processor {
    unsigned long long instructions_executed;
    unsigned long text_length;
    unsigned long stack_capacity;
    int debug_line;
    const char* text;
    char* stack;
    proc_state state;
    s_processor_registers registers;
    s_proc_vm* video_memory;
    
    unsigned long long video_memory_offset;
    
    char* memory;
} s_processor;

typedef struct proc_stack_argument {
    unsigned long long value;
    bool memory_access;
    unsigned long data_length;
    s_processor_register_types reg;
} s_proc_stack_argument;

void s_processor_stack_resize(s_processor* thou, int direction);

void s_processor_stack_resize_if_needed(s_processor* thou);

void s_processor_stack_push(s_processor* thou, void* ptr, unsigned long amount);

void s_processor_stack_pop(s_processor* thou, void* ptr, unsigned long amount);

void s_processor_stack_top(s_processor* thou, void* ptr, unsigned long amount);

void s_stack_destroy(s_processor* thou);

void compiler_previous_char(s_processor* thou);

unsigned char compiler_next_char(s_processor* thou);

void proc_error(s_processor* thou, proc_state error);

unsigned long long proc_read_integer(s_processor* thou, size_t bytes);

void proc_interpret(const char* text, unsigned long length);

void proc_dump(s_processor*);

#endif /* processor_h */
