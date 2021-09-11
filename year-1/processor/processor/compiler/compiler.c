
#include "errors.h"
#include "number_parser.h"
#include "command_names.h"
#include "command_handlers.h"
#include "compiler.h"

char compiler_braces_forward[256] = {
    ['('] = ')',
    ['['] = ']',
    ['{'] = '}'
};

char compiler_braces_backward[256] = {
    [')'] = '(',
    [']'] = '[',
    ['}'] = '{'
};

bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == ';';
}

bool is_digit_16(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
}

void compiler_out_buffer_extend(s_compiler* thou) {
    thou -> compiler_output_buffer_capacity *= 2;
    char* new_buffer = realloc(thou -> compiler_output_buffer, thou -> compiler_output_buffer_capacity);
    
    if(new_buffer == NULL) {
        compiler_fatal_error(thou, "compiler ran out of memory");
        return;
    }
    
    thou -> compiler_output_buffer = new_buffer;
}

void compiler_write_bytes(s_compiler* thou, const void* ptr, unsigned long bytes) {
    while(thou -> compiler_output_buffer_length + bytes > thou -> compiler_output_buffer_capacity) {
        compiler_out_buffer_extend(thou);
        if(thou -> state != COMPILER_STATE_RUNNING) return;
    }
    
    memcpy(thou -> compiler_output_buffer + thou -> compiler_output_buffer_length, ptr, bytes);
    thou -> compiler_output_buffer_length += bytes;
}

void compiler_write_byte(s_compiler* thou, char byte) {
    if(thou -> compiler_output_buffer_length + 1 > thou -> compiler_output_buffer_capacity) {
        compiler_out_buffer_extend(thou);
        if(thou -> state != COMPILER_STATE_RUNNING) return;
    }
    
    thou -> compiler_output_buffer[thou -> compiler_output_buffer_length++] = byte;
}

void compiler_write_string(s_compiler* thou, const char* string) {
    
    char c = '\0';
    while((c = *string++) != '\0') {
        compiler_write_byte(thou, c);
    }
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

void compiler_read_token(s_compiler* thou, s_compiler_token* target) {
    target -> begin = thou -> position;
    char c = '\0';
    
    int max_brace_amount = 4;
    char braces[max_brace_amount];
    int brace_amount = 0;
    
    do {
        c = compiler_next_char(thou);
        
        if(thou -> state == COMPILER_STATE_ERROR) {
            return;
        }
        
        if(compiler_braces_forward[c] != '\0') {
            
            if(brace_amount == max_brace_amount) {
                compiler_error(thou, "this expression is too complex");
                return;
            }
            
            braces[brace_amount] = compiler_braces_forward[c];
            brace_amount++;
            
        } else if(compiler_braces_backward[c] != '\0') {
            if(brace_amount == 0) {
                break;
            }
            brace_amount--;
            if(c != braces[brace_amount]) {
                compiler_error(thou, "'%c' expected", braces[max_brace_amount]);
                return;
            }
        } else if(brace_amount == 0) {
            if(!isalpha(c) && !isdigit(c) && c != '.' && c != '+' && c != '-' && c != ':' && c != '_') break;
        }
    } while(c != '\n' && c != '\0');
    
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

s_compiler_label* compiler_create_label(s_compiler* thou, s_compiler_token* token) {
    long token_length = token -> end.index - token -> begin.index;
    
    if(thou -> text[token -> end.index - 1] == ':') {
        token_length--; // Removing ":"
    }
    
    // Allocating space for label structure and label name
    
    s_compiler_label* label = calloc(1, sizeof(s_compiler_label) + token_length + 1);
    
    if(!label) {
        thou -> state = COMPILER_STATE_FATAL_ERROR;
        return NULL;
    }
    
    label -> position = 0;
    label -> resolved = false;
    label -> usages_buffer_length = 0;
    label -> usages_buffer_capacity = 16;
    label -> usages = calloc(label -> usages_buffer_capacity, sizeof(s_compiler_label_usage_point));
    
    if(!label) {
        free(label);
        thou -> state = COMPILER_STATE_FATAL_ERROR;
        return NULL;
    }
    
    label -> next = NULL;
    memcpy(label -> label, thou -> text + token -> begin.index, token_length);
    label -> label[token_length] = '\0';
    
    return label;
}

s_compiler_label* compiler_find_label(s_compiler* thou, s_compiler_token* token) {
    
    const char* token_start = thou -> text + token -> begin.index;
    long token_length = token -> end.index - token -> begin.index;
    
    if(thou -> text[token -> end.index - 1] == ':') {
        token_length--; // Removing ":"
    }
    
//    printf("token value is: ");
//
//    for(int i = 0; i < token_length; i++) {
//        printf("%c", thou -> text[i + token -> begin.index]);
//    }
//
//    printf("\n");
    
    for(s_compiler_label* label = thou -> label_list; label != NULL; label = label -> next) {
        
        int character_index = 0;
        
        const char* label_name = label -> label;
        
        for(character_index = 0; label_name[character_index] && character_index < token_length; character_index++) {
            if(label -> label[character_index] != token_start[character_index]) {
                break;
            }
        }
        
        if(character_index == token_length && label -> label[token_length] == '\0') return label;
    }
    
    return NULL;
}

void compiler_label_extend_usages_buffer(s_compiler* thou, s_compiler_label* label) {
    label -> usages_buffer_capacity *= 2;
    
    s_compiler_label_usage_point* new_buffer = realloc(label -> usages, label -> usages_buffer_capacity * sizeof(s_compiler_label_usage_point));
    
    if(new_buffer == NULL) {
        thou -> state = COMPILER_STATE_FATAL_ERROR;
        return;
    }
    
    label -> usages = new_buffer;
}

void compiler_label_add_usage(s_compiler* thou, s_compiler_label* label) {
    label -> usages_buffer_length++;
    
    if(label -> usages_buffer_length >= label -> usages_buffer_capacity) {
        compiler_label_extend_usages_buffer(thou, label);
    }
    
    s_compiler_label_usage_point* position = label -> usages + label -> usages_buffer_length - 1;
    
    position -> position = thou -> position;
    position -> address = thou -> compiler_output_buffer_length;
}

void compiler_label_destroy(s_compiler_label* label) {
    free(label -> usages);
    free(label);
}

void compiler_add_label(s_compiler* thou, s_compiler_label* label) {
    label -> next = thou -> label_list;
    thou -> label_list = label;
}

void compiler_extend_command_position_buffer(s_compiler* thou) {
    thou -> command_positions_buffer_length *= 2;
    
    s_compiler_command_position* new_buffer = realloc(thou -> command_positions, thou -> command_positions_buffer_length * sizeof(s_compiler_command_position));
    
    if(new_buffer == NULL) {
        thou -> state = COMPILER_STATE_FATAL_ERROR;
        thou -> command_positions_buffer_length /= 2;
        return;
    }
    
    thou -> command_positions = new_buffer;
}

void compiler_add_command_position(s_compiler* thou, s_compiler_command_position* position) {
    thou -> command_amount++;
    if(thou -> command_amount >= thou -> command_positions_buffer_length) {
        compiler_extend_command_position_buffer(thou);
        
        if(thou -> state != COMPILER_STATE_RUNNING) return;
    }
    
    thou -> command_positions[thou -> command_amount - 1] = *position;
}

void compiler_read_next_label(s_compiler* thou, s_compiler_token* token) {
    
    char ahead = compiler_ahead_char(thou);
    
    if(ahead != '\n' && ahead != '\0') {
        compiler_error(thou, "expected line end after a label");
        thou -> state = COMPILER_STATE_ERROR;
        return;
    }
    
    s_compiler_label* label = compiler_find_label(thou, token);
    
    if(label && label -> resolved) {
        compiler_error(thou, "'%s' label is already defined", label -> label);
        thou -> state = COMPILER_STATE_ERROR;
        return;
    } else if(label == NULL) {
        label = compiler_create_label(thou, token);
        compiler_add_label(thou, label);
    }
    
    label -> position = thou -> compiler_output_buffer_length;
    label -> resolved = true;
}

void compiler_read_next_line(s_compiler* thou) {
    compiler_skip_whitespaces(thou);
    char next = compiler_ahead_char(thou);
    
    if(next != '\n' && next != '\0') {
        
        s_compiler_command_position position;
        
        // Anchoring token start
        
        position.is_erroneous = false;
        position.command_byte_position = thou -> compiler_output_buffer_length;
        position.command_text_position = thou -> position.index;
        
        s_compiler_token token = {0};
        compiler_read_token(thou, &token);
        compiler_skip_whitespaces(thou);
        
        if(thou -> state != COMPILER_STATE_RUNNING) {
            return;
        }
        
        if(thou -> text[token.end.index - 1] == ':') {
            compiler_read_next_label(thou, &token);
            
            position.command_byte_length = 0;
            position.command_text_length = thou -> position.index - position.command_text_position;
            
            compiler_add_command_position(thou, &position);
        } else {
            
            if(thou -> config -> insert_line_flags) {
                
                compiler_put_line_flag(thou);
                
                position.command_text_length = 0;
                position.command_byte_length = thou -> compiler_output_buffer_length - position.command_byte_position;
                
                compiler_add_command_position(thou, &position);
            }
            
            position.command_byte_position = thou -> compiler_output_buffer_length;
            
            compiler_read_next_command(thou, &token);
            
            bool is_empty_line = false;
            
            position.command_byte_length = thou -> compiler_output_buffer_length - position.command_byte_position;
            position.command_text_length = thou -> position.index - position.command_text_position;
            
            if(thou -> state == COMPILER_STATE_ERROR) {
                // Errorneous line, rewinding the output buffer
                
                thou -> compiler_output_buffer_length = position.command_byte_position;
                
                position.command_byte_length = 0;
                position.is_erroneous = true;
                
                thou -> state = COMPILER_STATE_RUNNING;
                
            } else if(position.command_byte_length == 0) {
                // Empty line, don't write it to the command buffer
                
                is_empty_line = true;
            }
            
            if(!is_empty_line) {
                compiler_add_command_position(thou, &position);
            }
        }
    }
}

void compiler_read_next_command(s_compiler* thou, s_compiler_token* token) {
    
    long token_length = token -> end.index - token -> begin.index;
    
    const char* flag_delimiter_position = memchr(thou -> text + token -> begin.index, '.', token_length);
    
    long command_length = 0;
    long flag_length = 0;
    
    if(flag_delimiter_position == NULL) {
        command_length = token_length;
    } else {
        command_length = flag_delimiter_position - (thou -> text + token -> begin.index);
        flag_length = token_length - command_length - 1;
    }
    
    if(command_length <= 0) {
        compiler_error_positioned(thou, &token -> begin, "empty instruction");
        return;
    }
    
    if(command_length > COMPILER_MAX_COMMAND_LENGTH) {
        compiler_error_positioned(thou, &token -> begin, "too long instruction word");
        return;
    }
    
    if(flag_length > COMPILER_MAX_FLAG_LENGTH) {
        compiler_error_positioned(thou, &token -> begin, "instruction flag is too long");
        return;
    }
    
    // Allocating memory in stack to avoid malloc() call
    
    char command_name[COMPILER_MAX_COMMAND_LENGTH + 1];
    
    strncpy(command_name, thou -> text + token -> begin.index, command_length);
    command_name[command_length] = '\0';
    
    proc_command command = compiler_find_enum_index(command_name, COMPILER_COMMAND_NAMES);
    
    if(command == -1) {
        compiler_error_positioned(thou, &token -> begin, "unknown instruction: '%s'", command_name);
        return;
    }
    
    e_proc_flags flag = 0;
    
    unsigned long long command_flags = PROC_COMMAND_FLAGS[command];
    
    if(flag_delimiter_position != NULL) {
        char flag_name[COMPILER_MAX_FLAG_LENGTH + 1];
        
        strncpy(flag_name, thou -> text + token -> begin.index + command_length + 1, flag_length);
        flag_name[flag_length] = '\0';
        
        flag = compiler_find_enum_index(flag_name, COMPILER_FLAG_NAMES);
        
        if(flag == -1) {
            s_compiler_position token_position = token -> begin;
            token_position.ch += command_length + 2;
            
            compiler_error_positioned(thou, &token_position, "unknown flag: '%s'", flag_name);
            return;
        }
        
        unsigned long long flag_mask = 1 << flag;
        
        if((command_flags & flag_mask) == 0) {
            s_compiler_position token_position = token -> begin;
            token_position.ch += command_length + 2;
            
            compiler_error_positioned(thou, &token_position, "'%s' instruction cannot be used with '%s' flag", command_name, flag_name);
            return;
        }
    } else {
        if(command_flags != 0 && !(command_flags & PROC_FLAG_MASK_NONE)) {
            compiler_error(thou, "'%s' instruction should have a flag", command_name);
            return;
        }
        flag = PROC_FLAG_NONE;
    }
    
    switch(command) {
#define ADD_COMMAND(enum_name, command_name, command_flags, compile, process, decompile)    \
        case enum_name:                                                                     \
            compile                                                                         \
            break;
#include "../shared/command_pool.h"
    }
}

void compiler_put_line_flag(s_compiler* thou) {
    char code = PROC_COMMAND_FLAG_LINE;
    
    compiler_write_byte(thou, code);
    compiler_write_bytes(thou, &thou -> position.line, sizeof(int));
}

void compiler_print_listing_line(s_compiler* thou, s_compiler_command_position* command_position) {
    const int max_command_bytes = 11;
    
    fprintf(thou -> config -> listing_file, "| 0x%08llx | ", command_position -> command_byte_position);
    
    if(command_position -> is_erroneous) {
                                               // ff ff ff ff ff ff ff ff ff ff ff //
        fprintf(thou -> config -> listing_file, "          < error line >         ");
    } else {
        int bytes_written = 0;
        
        unsigned long long position = command_position -> command_byte_position;
        
        for(int i = 0; i < command_position -> command_byte_length; i++) {
            fprintf(thou -> config -> listing_file, "%02x ", (unsigned char)thou -> compiler_output_buffer[position]);
            position++;
            bytes_written++;
        }
        
        while(bytes_written++ < max_command_bytes) {
            fprintf(thou -> config -> listing_file, "   ");
        }
    }
    
    fprintf(thou -> config -> listing_file, "| ");
}

void compiler_print_listing(s_compiler* thou) {
    
    for(int i = 0; i < thou -> command_amount; i++) {
        s_compiler_command_position* command_position = thou -> command_positions + i;
        
        compiler_print_listing_line(thou, command_position);
        
        fwrite(thou -> text + command_position -> command_text_position, 1, command_position -> command_text_length, thou -> config -> listing_file);
        fprintf(thou -> config -> listing_file, "\n");
    }
}

bool compiler_perform(s_compiler_config* config) {
    bool success = true;
    
    const char* walker = config -> file;
    
    s_compiler thou = {0};
    
    thou.position.ch = 0;
    thou.position.line = 1;
    thou.position.index = 0;
    thou.position.filename = config -> full_path;
    thou.state = COMPILER_STATE_RUNNING;
    thou.config = config;
    thou.text = walker;
    thou.line_warning_emitted = false;
    
    thou.compiler_output_buffer_capacity = 1024;
    thou.compiler_output_buffer = calloc(thou.compiler_output_buffer_capacity, 1);
    thou.compiler_output_buffer_length = 0;
    
    thou.command_amount = 0;
    thou.command_positions_buffer_length = 1024;
    thou.command_positions = calloc(thou.command_positions_buffer_length, sizeof(s_compiler_command_position));
    
    if(!thou.command_positions || !thou.compiler_output_buffer) {
        free(thou.command_positions);
        free(thou.compiler_output_buffer);
        return false;
    }
    
    if(config -> put_shebang) {
        compiler_write_string(&thou, "#!/usr/bin/env eder\n");
    }
    
    compiler_write_bytes(&thou, &compiler_vendor, sizeof(uint32_t));
    compiler_write_bytes(&thou, &compiler_magic, sizeof(uint32_t));
    
    while(true) {
        
        compiler_read_next_line(&thou);
        
        if(thou.state == COMPILER_STATE_ERROR) {
            thou.state = COMPILER_STATE_RUNNING;
        }
        
        if(thou.state == COMPILER_STATE_FATAL_ERROR) {
            return false;
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
    
    for(s_compiler_label* label = thou.label_list; label != NULL; ) {
        if(label -> resolved) {
            char* jump_address_pointer = (char*)&label -> position;
            
            for(int i = 0; i < label -> usages_buffer_length; i++) {
                unsigned long long usage_address = label -> usages[i].address;
                
                for(int j = 0; j < sizeof(usage_address); j++) {
                    thou.compiler_output_buffer[usage_address + j] = jump_address_pointer[j];
                }
            }
        } else {
            success = false;
            for(int i = 0; i < label -> usages_buffer_length; i++) {
                compiler_error_positioned(&thou, &label -> usages[i].position, "label '%s' is undefined", label -> label);
            }
        }
        
        s_compiler_label* next = label -> next;
        compiler_label_destroy(label);
        label = next;
    }
    
    if(thou.command_amount > 0) {
        unsigned long long position = thou.command_positions[thou.command_amount - 1].command_byte_position;
        
        if(thou.compiler_output_buffer[position] != PROC_COMMAND_HLT) {
            compiler_warning(&thou, "the program does not end with the 'hlt' instruction");
        }
    }
    
    if(thou.config -> listing_file != NULL) {
        compiler_print_listing(&thou);
    }
    
    if(success) {
        fwrite(thou.compiler_output_buffer, 1, thou.compiler_output_buffer_length, thou.config -> output_file);
    }
    
    free(thou.compiler_output_buffer);
    free(thou.command_positions);
    
    return success;
}
