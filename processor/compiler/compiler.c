
#include "errors.h"
#include "number_parser.h"
#include "command_names.h"
#include "command_handlers.h"
#include "compiler.h"

void compiler_pipe_number(s_compiler* thou, e_proc_flags flag) {
    unsigned long long mask = 1 << flag;
    
    size_t flag_size = PROC_FLAG_SIZES[flag];
    
    if(mask & PROC_FLAG_MASK_INTS) {
        compiler_position position = thou -> position;
        u_compiler_number number = read_number(thou, true);
        
        // We cannot check if there is a 64-bit overflow
        
        if(flag_size != sizeof(unsigned long long)) {
            unsigned long bits = flag_size * 8;
            unsigned long long max_unsigned_value = (unsigned long long) 1 << bits;
            
            if(mask & PROC_FLAG_MASK_SIGNED_INTS) {
                max_unsigned_value /= 2;
                if(number.signed_value >= (long long)max_unsigned_value || number.signed_value < (long long)-max_unsigned_value) {
                    compiler_warning_positioned(thou, &position, "'%lld' exceeds %d-bit signed range (%lld to %lld)", number.unsigned_value, bits, -max_unsigned_value, max_unsigned_value - 1);
                }
            } else {
                if(number.unsigned_value >= max_unsigned_value) {
                    compiler_warning_positioned(thou, &position, "'%llu' exceeds %d-bit unsigned range (0 to %llu)", number.unsigned_value, bits, max_unsigned_value - 1);
                }
            }
        }
        
        if(mask & PROC_FLAG_MASK_SIGNED_INTS) {
            fwrite(&number.signed_value, flag_size, 1, thou -> output_stream);
        } else {
            fwrite(&number.unsigned_value, flag_size, 1, thou -> output_stream);
        }
    } else if(mask & PROC_FLAG_MASK_FLOATS) {
        u_compiler_number number = read_number(thou, false);
        
        if(flag == PROC_FLAG_FLOAT32) {
            float value = (float)number.double_value;
            
            fwrite(&value, flag_size, 1, thou -> output_stream);
        } else {
            fwrite(&number.double_value, flag_size, 1, thou -> output_stream);
        }
    }
}

bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == ';';
}

bool is_digit_16(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
}

void compiler_previous_char(s_compiler* thou) {
    if(thou -> position.ch == 0) return;
    thou -> position.ch--;
    thou -> position.index--;
}

char compiler_next_char(s_compiler* thou) {
    thou -> position.ch++;
    
    char result = thou -> text[thou -> position.index];
    thou -> position.index++;
    
    return result;
}

char compiler_ahead_char(s_compiler* thou) {
    return thou -> text[thou -> position.index];
}

void compiler_skip_whitespaces(s_compiler* thou) {
    
    char c = compiler_next_char(thou);
    bool is_comment = c == ';';
    
    if(is_whitespace(c) || is_comment) {
        do {
            c = compiler_next_char(thou);
            if(c == ';') is_comment = true;
        } while(is_whitespace(c) || (is_comment && c != '\n' && c != '\0'));
        
    }
    
    compiler_previous_char(thou);
}

void compiler_read_until(s_compiler* thou, char expected) {
    char c = '\0';
    
    do {
        c = compiler_next_char(thou);
        if(thou -> state == COMPILER_STATE_ERROR) {
            return;
        }
    } while(c != expected && c != '\n' && c != '\0');
    
    compiler_previous_char(thou);
    
    if(expected != '\n') {
        if(c == '\n') {
            compiler_error(thou, "unexpected newline");
        }
        if(c == '\0') {
            compiler_error(thou, "unexpected end of file");
        }
    }
}

void compiler_read_token(s_compiler* thou, compiler_token* target) {
    target -> begin = thou -> position;
    char c = '\0';
    
    do {
        c = compiler_next_char(thou);
        if(thou -> state == COMPILER_STATE_ERROR) {
            return;
        }
    } while(c != '\n' && !is_whitespace(c));
    
    compiler_previous_char(thou);
    
    target -> end = thou -> position;
}

int compiler_find_enum_index(const char* name, const char** string_list) {
    int list_index = 0;
    
    bool command_found = false;
    
    do {
        const char* command_name = string_list[list_index];
        int command_name_index = 0;
        long index = 0;
        bool match = true;
        
        while(name[index] && command_name[command_name_index]) {
            if(name[index] != command_name[command_name_index++]) {
                match = false;
                break;
            }
            
            index++;
        }
        
        if(command_name[command_name_index] != '\0' || name[index] != '\0') {
            match = false;
        }
        
        if(match) {
            return list_index;
            command_found = true;
            break;
        }
        
        list_index++;
        
    } while(string_list[list_index] != 0x00);
    
    return -1;
}

void read_command(s_compiler* thou) {
    
    compiler_skip_whitespaces(thou);
    char next = compiler_next_char(thou);
    compiler_previous_char(thou);
    
    if(next == '\n' || next == '\0') return; // Empty line
    
    if(thou -> state == COMPILER_STATE_ERROR) {
        return;
    }
    
    compiler_token token = {0};
    
    compiler_read_token(thou, &token);
    
    compiler_skip_whitespaces(thou);
    
    long token_length = token.end.index - token.begin.index;
    
    const char* flag_delimiter_position = memchr(thou -> text + token.begin.index, '.', token_length);
    
    long command_length = 0;
    long flag_length = 0;
    
    if(flag_delimiter_position == NULL) {
        command_length = token_length;
    } else {
        command_length = flag_delimiter_position - (thou -> text + token.begin.index);
        flag_length = token_length - command_length - 1;
    }
    
    if(command_length <= 0) {
        compiler_error_positioned(thou, &token.begin, "empty instruction");
        return;
    }
    
    if(command_length > COMPILER_MAX_COMMAND_LENGTH) {
        compiler_error_positioned(thou, &token.begin, "too long instruction word");
        return;
    }
    
    if(flag_length > COMPILER_MAX_FLAG_LENGTH) {
        compiler_error_positioned(thou, &token.begin, "instruction flag is too long");
        return;
    }
    
    // Allocating memory in stack to avoid malloc() call
    
    char command_name[COMPILER_MAX_COMMAND_LENGTH + 1];
    
    strncpy(command_name, thou -> text + token.begin.index, command_length);
    command_name[command_length] = '\0';
    
    proc_command command = compiler_find_enum_index(command_name, COMPILER_COMMAND_NAMES);
    
    if(command == -1) {
        compiler_error_positioned(thou, &token.begin, "unknown instruction: '%s'", command_name);
        return;
    }
    
    e_proc_flags flag = 0;
    
    if(flag_delimiter_position != NULL) {
        char flag_name[COMPILER_MAX_FLAG_LENGTH + 1];
        
        strncpy(flag_name, thou -> text + token.begin.index + command_length + 1, flag_length);
        flag_name[flag_length] = '\0';
        
        flag = compiler_find_enum_index(flag_name, COMPILER_FLAG_NAMES);
        
        if(flag == -1) {
            compiler_position token_position = token.begin;
            token_position.ch += command_length + 2;
            
            compiler_error_positioned(thou, &token_position, "unknown flag: '%s'", flag_name);
            return;
        }
        
        unsigned long long flag_mask = 1 << flag;
        unsigned long long command_flags = PROC_COMMAND_FLAGS[command];
        
        if((command_flags & flag_mask) == 0) {
            compiler_position token_position = token.begin;
            token_position.ch += command_length + 2;
            
            compiler_error_positioned(thou, &token_position, "'%s' instruction cannot be used with '%s' flag", command_name, flag_name);
            return;
        }
        
        PROC_COMMAND_HANDLERS[command](thou, flag);
    } else {
        PROC_COMMAND_HANDLERS[command](thou, PROC_FLAG_NONE);
    }
}

bool compile(const char* text, const char* filename, FILE* output_stream) {
    bool success = true;
    
    const char* walker = text;
    
    s_compiler thou = {0};
    
    thou.position.ch = 0;
    thou.position.line = 1;
    thou.position.index = 0;
    thou.position.filename = filename;
    thou.state = COMPILER_STATE_RUNNING;
    thou.output_stream = output_stream;
    thou.text = walker;
    
    fputs("#!/usr/bin/env eder\n", output_stream);
    
    
    fwrite(&compiler_vendor, 1, sizeof(uint32_t), output_stream);
    fwrite(&compiler_magic, 1, sizeof(uint32_t), output_stream);
    
    while(true) {
        read_command(&thou);
    
        if(thou.state == COMPILER_STATE_ERROR) {
            success = false;
            thou.state = COMPILER_STATE_RUNNING;
        }
        
        char next = compiler_next_char(&thou);
        
        if(next != '\n' && next != '\0') {
            compiler_read_until(&thou, '\n');
            next = compiler_next_char(&thou);
        }
        
        if(next == '\0') {
            break;
        }
        
        thou.position.ch = 0;
        thou.position.line ++;
    }
    
    return success;
}
