
#include "decompiler.h"

unsigned long long decompiler_read_integer(s_decompiler* thou, size_t bytes) {
    unsigned long long result = 0;
    unsigned long long shift = 0;
    
    for(int i = 0; i < bytes; i++) {
        result += (unsigned long long)decompiler_next_char(thou) << shift;
        shift += 8;
    }
    
    return result;
}

void decompiler_print_stack_argument(s_decompiler* thou, e_proc_flags flag) {
    if(thou -> state != DECOMPILER_STATE_RUNNING) return;
    
    bool register_met = flag & (1 << 4);
    bool number_met   = flag & (1 << 5);
    bool memory_read  = flag & (1 << 6);
    bool reg_first    = flag & (1 << 7);

    flag &= 0x0F;

    if(thou -> state != DECOMPILER_STATE_RUNNING) return;

    if(memory_read) {
        fputc('[', thou -> config -> output_file);
        flag = PROC_FLAG_INT64;
    } else if(register_met && number_met) {
        fputc('(', thou -> config -> output_file);
    }
    
    if(!reg_first) {
        e_proc_flags reg = decompiler_next_char(thou);
        decompiler_pipe_number(thou, flag);
        fprintf(thou -> config -> output_file, " - %s", COMPILER_REGISTER_NAMES[reg]);
    } else if(register_met) {
        e_proc_flags reg = decompiler_next_char(thou);
        fprintf(thou -> config -> output_file, "%s", COMPILER_REGISTER_NAMES[reg]);
    }

    if(number_met && reg_first) {
        unsigned long flag_size = PROC_FLAG_SIZES[flag];
        unsigned long long number = decompiler_read_integer(thou, flag_size);
        
        if(thou -> state != DECOMPILER_STATE_RUNNING) return;
        
        bool sign = true;
        
        if(register_met) fputc(' ', thou -> config -> output_file);
        
        if(flag == PROC_FLAG_FLOAT64) {
            if(*((double*)&number) < 0)
                *((double*)&number) = -*((double*)&number);
            else
                sign = false;
        } else if(flag == PROC_FLAG_FLOAT32) {
            if(*((float*)&number) < 0)
                *((float*)&number) = -*((float*)&number);
            else
                sign = false;
        } else if((number & (1 << (flag_size * 8 - 1))) > 0) {
            number = -number;
        } else {
            sign = false;
        }
        
        if(sign) fputc('-', thou -> config -> output_file);
        else if(register_met) fputc('+', thou -> config -> output_file);
        
        if(register_met) fputc(' ', thou -> config -> output_file);
        
        decompiler_print_number(thou, flag, number);
    }

    if(memory_read) {
        fputc(']', thou -> config -> output_file);
    } else if(register_met && number_met) {
        fputc(')', thou -> config -> output_file);
    }

    if(thou -> state != DECOMPILER_STATE_RUNNING) return;
}

void decompiler_print_number(s_decompiler* thou, e_proc_flags flag, unsigned long long buffer) {
    unsigned long flag_size = PROC_FLAG_SIZES[flag];
    unsigned long long mask = 1 << flag;
    
    if(mask & PROC_FLAG_MASK_SIGNED_INTS) {
        unsigned long long trimmer = 0;
        
        if(flag_size == sizeof(trimmer)) {
            trimmer = -1;
        } else {
            trimmer = ((unsigned long long)1 << (flag_size * 8)) - 1;
        }
        
        if(buffer & ((unsigned long long)1 << (flag_size * 8 - 1))) {
            buffer |= (~trimmer);
        } else {
            buffer &= trimmer;
        }
        
        fprintf(thou -> config -> output_file, "%lld", buffer);
    } else if(flag == PROC_FLAG_FLOAT32) {
        fprintf(thou -> config -> output_file, "%f", *((float*)&buffer));
    } else if(flag == PROC_FLAG_FLOAT64) {
        fprintf(thou -> config -> output_file, "%g", *((double*)&buffer));
    } else {
        
        if(flag_size < 8) {
            buffer &= ((unsigned long long)1 << (flag_size * 8)) - 1;
        }
        
        fprintf(thou -> config -> output_file, "%llu", buffer);
    }
}

void decompiler_pipe_number(s_decompiler* thou, e_proc_flags flag) {
    unsigned long flag_size = PROC_FLAG_SIZES[flag];
    unsigned long long argument = decompiler_read_integer(thou, flag_size);
    
    decompiler_print_number(thou, flag, argument);
}

void decompiler_previous_char(s_decompiler* thou) {
    thou -> position--;
}

unsigned char decompiler_char_ahead(s_decompiler* thou) {
    return thou -> input[thou -> position];
}

unsigned char decompiler_next_char(s_decompiler* thou) {
    unsigned char result = thou -> input[thou -> position];
    
    if(thou -> position + 1 < thou -> input_length) thou -> position++;
    else thou -> state = DECOMPILER_STATE_REACHED_FILE_END;
    
    return result;
}

void decompiler_process_command(s_decompiler* thou) {
    
    proc_command command = decompiler_char_ahead(thou);
    
    if(thou -> config -> ignore_line_flags && command == PROC_COMMAND_FLAG_LINE) {
        thou -> position += 5;
        return;
    }
    
    printf("%08llx | ", thou -> position);
    
    command = decompiler_next_char(thou);
    
    switch(command) {
#define ADD_COMMAND(enum_name, command_name, command_flags, compile, process, decompile)    \
                                                                                            \
        case enum_name:                                                                     \
            decompile                                                                       \
            break;
            
#include "../shared/command_pool.h"
#undef ADD_COMMAND
        default:
            printf("< unknown command >");
    }
    
    printf("\n");
}

bool decompiler_perform(s_decompiler_config* config) {
    s_decompiler thou = {};
    
    thou.input = config -> input;
    thou.input_length = config -> input_length;
    thou.state = DECOMPILER_STATE_RUNNING;
    thou.config = config;
    thou.position = 0;
    
    // Skip shebang line
    
    if(decompiler_next_char(&thou) == '#') {
        fprintf(config -> output_file, "Shebang: '");
        fputc('#', config -> output_file);
        char c;
        while((c = decompiler_next_char(&thou)) != '\n') {
            fputc(c, config -> output_file);
            if(c == '\0') return false;
        }
        fprintf(config -> output_file, "'\n");
    } else {
        decompiler_next_char(&thou);
    }
    
    uint32_t readen_vendor = (uint32_t)decompiler_read_integer(&thou, sizeof(uint32_t));
    uint32_t readen_magic = (uint32_t)decompiler_read_integer(&thou, sizeof(uint32_t));
    
    printf("File compiler version: v%d.%d.%d.%d\n", (char)(readen_vendor   >> 24), (char)(readen_vendor   >> 16), (char)(readen_vendor)   >> 8, (char)readen_vendor);
    printf("File magic number: 0x%08x\n", readen_magic);
    
    if(compiler_vendor != readen_vendor) {
        printf("Unable to decompile this file: this binary file was built with compiler v%d.%d.%d.%d, but the decompiler is v%d.%d.%d.%d\n",
               (char)(readen_vendor   >> 24), (char)(readen_vendor   >> 16), (char)(readen_vendor)   >> 8, (char)readen_vendor,
               (char)(compiler_vendor >> 24), (char)(compiler_vendor >> 16), (char)(compiler_vendor) >> 8, (char)compiler_vendor);
        
        return false;
    }
    
    if(compiler_magic != readen_magic) {
        printf("Unable to decompiler this file: this binary file has invalid magic number: 0x%08x instead of 0x%08x\n", readen_magic, compiler_magic);
        return false;
    }
    
    while(thou.state == DECOMPILER_STATE_RUNNING) {
        decompiler_process_command(&thou);
    }
    
    return true;
}
