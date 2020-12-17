
#include "compiler.h"
#include "errors.h"
#include "commands.h"
#include "number_parser.h"
#include "command_handlers.h"
#include "register_names.h"
#include "../shared/registers.h"

void proc_parse_stack_argument(s_compiler* thou, e_proc_flags flag, s_stack_argument* result, proc_command command) {
    
    bool register_met = false;
    bool number_met = false;
    bool reg_first = true;
    bool memory_read = false;
    char sign = '\0';
    
    u_compiler_number number = {0};
    register_t reg = 0;
    
    bool braces = false;
    
    s_compiler_token token;
    
    compiler_read_token(thou, &token);
    
    thou -> position = token.begin;
    
    if(compiler_ahead_char(thou) == '(') compiler_next_char(thou);

    while(thou -> position.index < token.end.index) {
        char next_character = compiler_next_char(thou);
        
        if(next_character == '(') {
            compiler_error(thou, "'(' may only appear as the argument beginning");
            return;
            
        } else if(next_character == ')') {
            if(thou -> position.index == token.end.index) {
                break;
            }
            
            compiler_error(thou, "')' may only appear as the argument ending");
            return;
        }
        
        if(is_whitespace(next_character)) {
            continue;
        }
        
        if(!braces && memory_read) {
            compiler_error(thou, "the dereferencing operator must include the whole argument");
            return;
        }
        
        if(next_character == '[') {
            if(memory_read) {
                compiler_error(thou, "nested dereferencing is not allowed");
                return;
            }
            if(register_met || sign || number_met) {
                compiler_error(thou, "the dereferencing operator must include the whole argument");
                return;
            }
            braces = true;
            memory_read = true;
            continue;
        }
        if(next_character == ']') {
            braces = false;
            continue;
        }
        
        if(number_met && register_met) {
            if(memory_read) {
                compiler_error(thou, "expected ']'");
            } else {
                compiler_error(thou, "expected ')'");
            }
            return;
        }
        
        if(next_character == '+' || next_character == '-') {
            if(sign) {
                compiler_error(thou, "duplicating sign");
                return;
            }
            sign = next_character;
        } else if(isalpha(next_character)) {
            if(register_met) {
                compiler_error(thou, "expected number");
                return;
            }
            if(number_met && !sign) {
                compiler_error(thou, "missing '+' or '-' between register and number");
                return;
            }
            if(sign && !number_met) {
                compiler_error(thou, "cannot use '%c' on register", sign);
                return;
            }
            if(number_met && sign == '-') {
                reg_first = false;
            }
            
            compiler_previous_char(thou);
            
            reg = proc_parse_register_name(thou);
            if(thou -> state != COMPILER_STATE_RUNNING) return;
            
            register_met = true;
        } else if(isdigit(next_character)) {
            if(number_met) {
                if(sign) {
                    compiler_error(thou, "expected register name");
                } else {
                    compiler_error(thou, "expected '+', '-' or '%c'", memory_read ? ']' : ')');
                }
                
                return;
            }
            if(register_met && !sign) {
                compiler_error(thou, "missing '+' or '-' between register and number");
                return;
            }
            
            unsigned long long mask = (1 << flag);
            
            compiler_previous_char(thou);
            
            bool is_number = memory_read || (mask & PROC_FLAG_MASK_FLOATS) == 0;
            
            number = read_number(thou, is_number);
            
            if(thou -> state != COMPILER_STATE_RUNNING) return;
            
            if(sign == '-') {
                if(mask & PROC_FLAG_MASK_FLOATS) number.double_value = -number.double_value;
                else number.unsigned_value = -number.unsigned_value;
            }
            sign = '\0';
            
            number_met = true;
        }
    }
    
    if(!register_met && !number_met) {
        compiler_error(thou, "expected number or register name");
        return;
    }

    if(flag != PROC_FLAG_NONE && register_met) {
        unsigned long reg_size = PROC_REGISTER_SIZES[reg];
        unsigned long flag_size = PROC_FLAG_SIZES[flag];
        
        if(register_met && !memory_read && reg_size < flag_size) {
            compiler_error_positioned(thou, &token.begin, "cannot treat %d-byte register as %d-byte value", reg_size, flag_size);
            return;
        }
    }
    
    if(command == PROC_COMMAND_POP) {
        if(register_met && number_met && !memory_read) {
            compiler_error(thou, "dereferencing operator seems to have been missed");
            return;
        }
        if(number_met && !memory_read && !register_met) {
            compiler_error(thou, "i don't understand what do you want me to do");
            return;
        }
    }
    
    if(flag == PROC_FLAG_NONE) {
        if((!register_met || memory_read)) {
            compiler_error_positioned(thou, &token.begin, "value type flag is required when the data size cannot be determined");
            return;
        }
        
        unsigned long reg_size = PROC_REGISTER_SIZES[reg];
        
        if(reg_size == 8)
            flag = PROC_FLAG_UNSIGNED_INT64;
        else if(reg_size == 4)
            flag = PROC_FLAG_UNSIGNED_INT32;
        else if(reg_size == 2)
            flag = PROC_FLAG_UNSIGNED_INT16;
        else
            flag = PROC_FLAG_UNSIGNED_INT8;
    }
    
    flag |= register_met << 4;
    flag |= number_met   << 5;
    flag |= memory_read  << 6;
    flag |= reg_first    << 7;
    
    result -> flag = flag;
    result -> register_met = register_met;
    result -> number_met = number_met;
    result -> memory_read = memory_read;
    result -> reg_first = reg_first;
    
    result -> number = number;
    result -> reg = reg;
}

int proc_parse_register_name(s_compiler* thou) {
    s_compiler_token token = {0};
    compiler_read_token(thou, &token);
    
    if(thou -> state != COMPILER_STATE_RUNNING) return -1;
    
    long token_length = token.end.index - token.begin.index;
    
    if(token_length > COMPILER_MAX_REGISTER_LENGTH) {
        compiler_error_positioned(thou, &token.begin, "register name is too long");
        return -1;
    }
    
    char register_name[COMPILER_MAX_REGISTER_LENGTH + 1];
    register_name[token_length] = '\0';
    
    memcpy(register_name, thou -> text + token.begin.index, token_length);
    
    int register_index = compiler_find_enum_index(register_name, COMPILER_REGISTER_NAMES);
    
    if(register_index == -1) {
        compiler_error_positioned(thou, &token.begin, "unknown register: %s", register_name);
        return -1;
    }
    
    return register_index;
}

void proc_add_jmp_instruction(s_compiler* thou, proc_command command, e_proc_flags flag) {
    compiler_write_byte(thou, command);
    if(flag != PROC_FLAG_NONE) {
        compiler_write_byte(thou, flag);
    }
    
    char c = compiler_ahead_char(thou);
    
    if(isdigit(c)) {
        compiler_pipe_number(thou, PROC_FLAG_UNSIGNED_INT64);
    } else if(isalpha(c) || c == '_') {
        unsigned long long address = 0;
        
        s_compiler_token token = {};

        compiler_read_token(thou, &token);
        
        s_compiler_label* label = compiler_find_label(thou, &token);
        
        if(label == NULL) {
            label = compiler_create_label(thou, &token);
            if(thou -> state != COMPILER_STATE_RUNNING) return;
            compiler_add_label(thou, label);
        }
        if(label -> resolved) {
            address = label -> position;
        } else {
            compiler_label_add_usage(thou, label);
        }
        
        compiler_write_bytes(thou, &address, 8);
    } else {
        compiler_error(thou, "expected address or label");
    }
}
