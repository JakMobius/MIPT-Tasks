

#ifndef instruction_handlers_h
#define instruction_handlers_h

#include <stdbool.h>

void proc_push_instruction_handler(s_processor* thou, bool is_register);

void proc_pop_instruction_handler(s_processor* thou);

void proc_math_instruction_handler(s_processor* thou, char op);

void proc_in_instruction_handler(s_processor* thou);

void proc_out_instruction_handler(s_processor* thou);

void proc_sqrt_instruction_handler(s_processor* thou);


#endif /* instruction_handlers_h */
