
#ifndef command_handlers_h
#define command_handlers_h

#include <stdio.h>

typedef struct stack_argument {
    u_compiler_number number;
    register_t reg;
    unsigned char flag;
    
    bool reg_first; // This is false for operations like [1 - reg] or (1 - reg)
    bool register_met;
    bool number_met;
    bool memory_read;
} s_stack_argument;

void proc_parse_stack_argument(s_compiler* thou, e_proc_flags flag, s_stack_argument* result, proc_command command);

int proc_parse_register_name(s_compiler* thou);

void proc_add_jmp_instruction(s_compiler* thou, proc_command command, e_proc_flags flag);

#endif /* command_handlers_h */
