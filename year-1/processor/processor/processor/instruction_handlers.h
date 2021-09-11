

#ifndef instruction_handlers_h
#define instruction_handlers_h

#include <stdbool.h>

void proc_read_stack_argument(s_processor* thou, s_proc_stack_argument* argument);

void proc_math_instruction_handler(s_processor* thou, char op);

bool proc_argument_zero(s_processor* thou, e_proc_flags flag);

int proc_argument_relation(s_processor* thou, e_proc_flags flag);

#endif /* instruction_handlers_h */
