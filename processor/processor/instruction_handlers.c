
#include "processor.h"
#include "../shared/commands.h"
#include "instruction_handlers.h"

void proc_push_instruction_handler(s_processor* thou, bool is_register) {
    unsigned char flag = compiler_next_char(thou);
    
    if(is_register) {
        unsigned long register_size = PROC_REGISTER_SIZES[flag];
        unsigned long register_offset = PROC_REGISTER_OFFSETS[flag];
        unsigned long long register_value = 0;
        
        char* register_address = (char*)(&thou -> registers);
        
        memcpy(&register_value, register_address + register_offset, register_size);
        
        s_processor_stack_push(thou, &register_value, register_size);
        return;
    }
    
    unsigned long flag_size = PROC_FLAG_SIZES[flag];
    unsigned long long argument = read_integer(thou, flag_size);
    
    if(thou -> state == PROC_STATE_REACHED_FILE_END) {
        proc_error(thou, thou -> state);
        return;
    }
        
    s_processor_stack_push(thou, &argument, flag_size);
}

void proc_pop_instruction_handler(s_processor* thou) {
    unsigned char flag = compiler_next_char(thou);
    unsigned long register_size = PROC_REGISTER_SIZES[flag];
    unsigned long register_offset = PROC_REGISTER_OFFSETS[flag];
    unsigned long long register_value = 0;
    
    s_processor_stack_pop(thou, &register_value, register_size);
    
    if(thou -> state != PROC_STATE_RUNNING) {
        return;
    }
    
    char* register_address = (char*)(&thou -> registers);
    
    memcpy(register_address + register_offset, &register_value, register_size);
}

void proc_math_instruction_handler(s_processor* thou, char op) {
    unsigned char flag = compiler_next_char(thou);
    unsigned long flag_size = PROC_FLAG_SIZES[flag];
    
    if(thou -> state == PROC_STATE_REACHED_FILE_END) {
        proc_error(thou, thou -> state);
        return;
    }
    
    unsigned long long a, b;
    
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

void proc_in_instruction_handler(s_processor* thou) {
    unsigned char flag = compiler_next_char(thou);
    unsigned long flag_size = PROC_FLAG_SIZES[flag];
    
    if(thou -> state == PROC_STATE_REACHED_FILE_END) {
        proc_error(thou, thou -> state);
        return;
    }
    
    unsigned long long buffer = 0;
        
    unsigned long long mask = 1 << flag;
    
    if(mask & PROC_FLAG_MASK_SIGNED_INTS) {
        scanf("%lld", &buffer);
    } else if(mask & PROC_FLAG_MASK_UNSIGNED_INTS) {
        scanf("%llu", &buffer);
    } else if(flag == PROC_FLAG_FLOAT32) {
        scanf("%f", ((float*)&buffer));
    } else if(flag == PROC_FLAG_FLOAT64) {
        scanf("%lg", ((double*)&buffer));
    }
    
    s_processor_stack_push(thou, &buffer, flag_size);
        
    if(thou -> state != PROC_STATE_RUNNING) return;
}

void proc_out_instruction_handler(s_processor* thou) {
    unsigned char flag = compiler_next_char(thou);
    unsigned long long flag_size = PROC_FLAG_SIZES[flag];
    
    if(thou -> state == PROC_STATE_REACHED_FILE_END) {
        proc_error(thou, thou -> state);
        return;
    }
    
    unsigned long long buffer = 0;
    
    s_processor_stack_top(thou, &buffer, flag_size);
        
    if(thou -> state != PROC_STATE_RUNNING) return;
        
    unsigned long long mask = 1 << flag;
    unsigned long long trimmer = 0;
    
    if(flag_size == sizeof(trimmer)) {
        trimmer = -1;
    } else {
        trimmer = ((unsigned long long)1 << (flag_size * 8));
    }
    
    if(mask & PROC_FLAG_MASK_SIGNED_INTS) {
        printf("%lld\n", (signed long long) buffer & trimmer);
    } else if(mask & PROC_FLAG_MASK_UNSIGNED_INTS) {
        printf("%llu\n", (unsigned long long) buffer & trimmer);
    } else if(flag == PROC_FLAG_FLOAT32) {
        printf("%f\n", *((float*)&buffer));
    } else if(flag == PROC_FLAG_FLOAT64) {
        printf("%lg\n", *((double*)&buffer));
    }
}

void proc_sqrt_instruction_handler(s_processor* thou) {
    unsigned char flag = compiler_next_char(thou);
    unsigned long flag_size = PROC_FLAG_SIZES[flag];
    
    if(thou -> state == PROC_STATE_REACHED_FILE_END) {
        proc_error(thou, thou -> state);
        return;
    }
    
    unsigned long long number;
    
    s_processor_stack_pop(thou, &number, flag_size);
    
    if(thou -> state != PROC_STATE_RUNNING) return;
    
    unsigned long long result;
    
    unsigned long long trimmer = 0;
    
    if(flag_size == sizeof(trimmer)) {
        trimmer = -1;
    } else {
        trimmer = ((unsigned long long)1 << (flag_size * 8));
    }
    
    if(flag == PROC_FLAG_FLOAT32) {
        float casted = *((float*)&number);
        if(casted < 0) {
            proc_error(thou, PROC_STATE_MATHEMATICAL_EXCEPTION);
            return;
        }
        *((float*)&result) = sqrtf(casted);
    } else if(flag == PROC_FLAG_FLOAT64) {
        double casted = *((double*)&number);
        if(casted < 0) {
            proc_error(thou, PROC_STATE_MATHEMATICAL_EXCEPTION);
            return;
        }
        *((double*)&result) = sqrt(casted);
    }
    
    s_processor_stack_push(thou, &result, flag_size);
}
