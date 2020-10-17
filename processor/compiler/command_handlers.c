
#include "compiler.h"
#include "errors.h"
#include "commands.h"
#include "number_parser.h"
#include "command_handlers.h"
#include "register_names.h"
#include "../shared/registers.h"

void proc_push_command_handler(s_compiler* thou, e_proc_flags flags);
void proc_pop_command_handler(s_compiler* thou, e_proc_flags flags);
void proc_add_command_handler(s_compiler* thou, e_proc_flags flags);
void proc_subtract_command_handler(s_compiler* thou, e_proc_flags flags);
void proc_multiply_command_handler(s_compiler* thou, e_proc_flags flags);
void proc_divide_command_handler(s_compiler* thou, e_proc_flags flags);
void proc_in_command_handler(s_compiler* thou, e_proc_flags flags);
void proc_out_command_handler(s_compiler* thou, e_proc_flags flags);
void proc_hlt_command_handler(s_compiler* thou, e_proc_flags flags);
void proc_nop_command_handler(s_compiler* thou, e_proc_flags flags);
void proc_sqrt_command_handler(s_compiler* thou, e_proc_flags flags);

void(*PROC_COMMAND_HANDLERS[])(s_compiler* thou, e_proc_flags flags) = {
    proc_push_command_handler,     // PROC_COMMAND_PUSH
    proc_pop_command_handler,      // PROC_COMMAND_POP
    proc_add_command_handler,      // PROC_COMMAND_ADD
    proc_subtract_command_handler, // PROC_COMMAND_SUBTRACT
    proc_multiply_command_handler, // PROC_COMMAND_MULTIPLY
    proc_divide_command_handler,   // PROC_COMMAND_DIVIDE
    proc_sqrt_command_handler,     // PROC_COMMAND_SQRT
    proc_in_command_handler,       // PROC_COMMAND_IN
    proc_out_command_handler,      // PROC_COMMAND_OUT
    proc_hlt_command_handler,      // PROC_COMMAND_HLT
    proc_nop_command_handler       // PROC_COMMAND_NOP
};

int proc_parse_register_name(s_compiler* thou) {
    compiler_token token = {0};
    compiler_read_token(thou, &token);
    
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

void proc_push_command_handler(s_compiler* thou, e_proc_flags flags) {
    
    char next_character = compiler_ahead_char(thou);
    
    if(!isalpha(next_character)) {
        char code = PROC_COMMAND_PUSH;
        
        fwrite(&code, 1, 1, thou -> output_stream);
        fwrite(&flags, 1, 1, thou -> output_stream);
        
        compiler_pipe_number(thou, flags);
    } else {
        
        if(flags != 0) {
            compiler_warning(thou, "flags are ignored when pushing to register");
        }
        
        char result_flag = proc_parse_register_name(thou);
        if(result_flag == -1) return;
        char code = PROC_CMNDXTN_PUSH_REGISTER;
        
        fwrite(&code, 1, 1, thou -> output_stream);
        fwrite(&result_flag, 1, 1, thou -> output_stream);
    }
}

void proc_pop_command_handler(s_compiler* thou, e_proc_flags flags) {
    char code = PROC_COMMAND_POP;
    
    char result_flag = proc_parse_register_name(thou);
    if(result_flag == -1) return;
    
    fwrite(&code, 1, 1, thou -> output_stream);
    fwrite(&result_flag, 1, 1, thou -> output_stream);
}

void proc_add_command_handler(s_compiler* thou, e_proc_flags flags) {
    char code = PROC_COMMAND_ADD;
    
    fwrite(&code, 1, 1, thou -> output_stream);
    fwrite(&flags, 1, 1, thou -> output_stream);
}

void proc_subtract_command_handler(s_compiler* thou, e_proc_flags flags) {
    char code = PROC_COMMAND_SUBTRACT;
    
    fwrite(&code, 1, 1, thou -> output_stream);
    fwrite(&flags, 1, 1, thou -> output_stream);
}

void proc_multiply_command_handler(s_compiler* thou, e_proc_flags flags) {
    char code = PROC_COMMAND_MULTIPLY;
    
    fwrite(&code, 1, 1, thou -> output_stream);
    fwrite(&flags, 1, 1, thou -> output_stream);
}

void proc_divide_command_handler(s_compiler* thou, e_proc_flags flags) {
    char code = PROC_COMMAND_DIVIDE;
    
    fwrite(&code, 1, 1, thou -> output_stream);
    fwrite(&flags, 1, 1, thou -> output_stream);
}

void proc_sqrt_command_handler(s_compiler* thou, e_proc_flags flags) {
    char code = PROC_COMMAND_SQRT;
    
    fwrite(&code, 1, 1, thou -> output_stream);
    fwrite(&flags, 1, 1, thou -> output_stream);
}

void proc_in_command_handler(s_compiler* thou, e_proc_flags flags) {
    char code = PROC_COMMAND_IN;
    
    fwrite(&code, 1, 1, thou -> output_stream);
    fwrite(&flags, 1, 1, thou -> output_stream);
}

void proc_out_command_handler(s_compiler* thou, e_proc_flags flags) {
    char code = PROC_COMMAND_OUT;
    
    fwrite(&code, 1, 1, thou -> output_stream);
    fwrite(&flags, 1, 1, thou -> output_stream);
}

void proc_hlt_command_handler(s_compiler* thou, e_proc_flags flags) {
    char code = PROC_COMMAND_HLT;
    
    fwrite(&code, 1, 1, thou -> output_stream);
}

void proc_nop_command_handler(s_compiler* thou, e_proc_flags flags) {
    char code = PROC_COMMAND_NOP;
    
    fwrite(&code, 1, 1, thou -> output_stream);
}
