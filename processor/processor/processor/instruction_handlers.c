
#include "processor.h"
#include "../shared/commands.h"
#include "instruction_handlers.h"

void proc_read_stack_argument(s_processor* thou, s_proc_stack_argument* argument) {
    unsigned char flag = compiler_next_char(thou);

    if(thou -> state != PROC_STATE_RUNNING) return;
    
    bool register_met = flag & (1 << 4);
    bool number_met   = flag & (1 << 5);
    bool memory_read  = flag & (1 << 6);
    bool reg_first    = flag & (1 << 7);

    flag &= 0x0F;

    s_processor_register_types reg = 0;
    unsigned long flag_size = PROC_FLAG_SIZES[flag];
    unsigned long read_size;
    
    if(memory_read) {
        read_size = 8;
    } else {
        read_size = PROC_FLAG_SIZES[flag];
    }

    unsigned long long result_value = 0;
    
    if(register_met) {
        reg = compiler_next_char(thou);
        
        unsigned long register_offset = PROC_REGISTER_OFFSETS[reg];
        
        if(thou -> state != PROC_STATE_RUNNING) return;
        
        char* register_address = (char*)(&thou -> registers) + register_offset;
        
        memcpy(&result_value, register_address, read_size);
    }
    
    if(number_met) {
        if(thou -> state != PROC_STATE_RUNNING) return;
        
        unsigned long long argument;
        
        if(memory_read) {
            argument = proc_read_integer(thou, 8);
        } else {
            argument = proc_read_integer(thou, read_size);
        }
        
        
        if(thou -> state != PROC_STATE_RUNNING) return;
        
        if(!reg_first) {
            if(flag == PROC_FLAG_FLOAT64) {
                *((double*)&result_value) = *((double*)&argument) - *((double*)&result_value);
            } else if(flag == PROC_FLAG_FLOAT32) {
                *((float*)&result_value) = *((float*)&argument) - *((float*)&result_value);
            } else {
                result_value = argument - result_value;
            }
        } else {
            if(flag == PROC_FLAG_FLOAT64) {
                *((double*)&result_value) += *((double*)&argument);
            } else if(flag == PROC_FLAG_FLOAT32) {
                *((float*)&result_value) += *((float*)&argument);
            } else {
                result_value += argument;
            }
        }
    }
    
    argument -> value = result_value;
    argument -> memory_access = memory_read;
    argument -> data_length = flag_size;
    argument -> reg = reg;
}

void proc_math_instruction_handler(s_processor* thou, char op) {
    unsigned char flag = compiler_next_char(thou);
    unsigned long flag_size = PROC_FLAG_SIZES[flag];
    
    unsigned long long a = 0, b = 0;
    
    s_processor_stack_pop(thou, &b, flag_size);
    
    if(thou -> state != PROC_STATE_RUNNING) return;
    
    s_processor_stack_pop(thou, &a, flag_size);
    
    if(thou -> state != PROC_STATE_RUNNING) return;
    
    unsigned long long result;
    
    if(op == 0) {
        if(flag == PROC_FLAG_FLOAT32) {
            *((float*)&result) = *((float*)&a) + *((float*)&b);
        } else if(flag == PROC_FLAG_FLOAT64) {
            *((double*)&result) = *((double*)&a) + *((double*)&b);
        } else {
            result = a + b;
        }
    } else if(op == 1) {
        if(flag == PROC_FLAG_FLOAT32) {
            *((float*)&result) = *((float*)&a) - *((float*)&b);
        } else if(flag == PROC_FLAG_FLOAT64) {
            *((double*)&result) = *((double*)&a) - *((double*)&b);
        } else {
            result = a - b;
        }
    } else if(op == 2) {
        if(flag == PROC_FLAG_FLOAT32) {
            *((float*)&result) = *((float*)&a) * *((float*)&b);
        } else if(flag == PROC_FLAG_FLOAT64) {
            *((double*)&result) = *((double*)&a) * *((double*)&b);
        } else {
            result = a * b;
        }
    } else if(op == 3) {
        if(flag == PROC_FLAG_FLOAT32) {
            if(*((float*)&b) == 0) {
                proc_error(thou, PROC_STATE_DIVISION_BY_ZERO);
                return;
            }
            *((float*)&result) = *((float*)&a) / *((float*)&b);
        } else if(flag == PROC_FLAG_FLOAT64) {
            if(*((double*)&b) == 0) {
                proc_error(thou, PROC_STATE_DIVISION_BY_ZERO);
                return;
            }
            *((double*)&result) = *((double*)&a) / *((double*)&b);
        } else {
            if(b == 0) {
                proc_error(thou, PROC_STATE_DIVISION_BY_ZERO);
                return;
            }
            result = a / b;
        }
    }
    
    s_processor_stack_push(thou, &result, flag_size);
}

bool proc_argument_zero(s_processor* thou, e_proc_flags flag) {
    unsigned long flag_size = PROC_FLAG_SIZES[flag];
    
    unsigned long long a = 0;
    
    s_processor_stack_pop(thou, &a, flag_size);
    
    if(thou -> state != PROC_STATE_RUNNING) return 0;
    
    if(flag == PROC_FLAG_FLOAT32) {
        if(*((float*)&a) == (float) 0) return true;
        return false;
    } else if(flag == PROC_FLAG_FLOAT64) {
        if(*((double*)&a) == (double) 0) return true;
        return false;
    }
    
    if(a == 0) return true;
    return false;
}

int proc_argument_relation(s_processor* thou, e_proc_flags flag) {
    unsigned long flag_size = PROC_FLAG_SIZES[flag];
    
    unsigned long long a = 0, b = 0;
    
    unsigned long long mask = 1 << flag;
    
    s_processor_stack_pop(thou, &b, flag_size);
    
    if(thou -> state != PROC_STATE_RUNNING) return 0;
    
    s_processor_stack_pop(thou, &a, flag_size);
    
    if(thou -> state != PROC_STATE_RUNNING) return 0;
    
    if(flag == PROC_FLAG_FLOAT32) {
        if(*((float*)&a) == *((float*)&b)) return 0;
        if(*((float*)&a) > *((float*)&b)) return 1;
        return -1;
    } else if(flag == PROC_FLAG_FLOAT64) {
        if(*((double*)&a) == *((double*)&b)) return 0;
        if(*((double*)&a) > *((double*)&b)) return 1;
        return -1;
    } else if(mask & PROC_FLAG_MASK_UNSIGNED_INTS){
        if(a == b) return 0;
        if(a > b) return 1;
        return -1;
    } else {
        if(a == b) return 0;
        unsigned long long signed_mask = ((unsigned long long) 1 << (flag_size * 8 - 1));
        
        bool a_ltz = a & signed_mask;
        bool b_ltz = b & signed_mask;
        
        if(a_ltz) {
            if(!b_ltz) return -1;
        } else {
            if(b_ltz) return 1;
        }
        
        if(a > b) return 1;
        return -1;
    }
}
