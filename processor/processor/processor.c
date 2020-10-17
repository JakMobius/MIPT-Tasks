
#include "../shared/commands.h"
#include "processor.h"
#include "instruction_handlers.h"


void s_processor_stack_resize(s_processor* thou, int direction) {
    
    unsigned long capacity = thou -> stack_capacity;
    
    if(direction > 0) capacity *= 2;
    else capacity /= 2;
    
    if(capacity < thou -> registers.rsp) capacity = thou -> registers.rsp;
    if(capacity < 16) capacity = 16;
    if(capacity == thou -> stack_capacity) return;
    
    char* buffer = (char*)realloc(thou -> stack, capacity);
    
    if(buffer == NULL) {
        if(direction > 0) capacity /= 2;
        else capacity *= 2;
        proc_error(thou, PROC_STATE_OUT_OF_MEMORY);
        return;
    }
    
    thou -> stack = buffer;
}

void s_processor_stack_resize_if_needed(s_processor* thou) {
    if(thou -> registers.rsp >= thou -> stack_capacity) {
        s_processor_stack_resize(thou, 1);
    } else if(thou -> registers.rsp * 2 < thou -> stack_capacity) {
        s_processor_stack_resize(thou, -1);
    }
}

void s_processor_stack_push(s_processor* thou, void* ptr, size_t amount) {
    //printf("push %lu\n", amount);
    thou -> registers.rsp += amount;
    
    s_processor_stack_resize_if_needed(thou);
    
    if(thou -> state != PROC_STATE_RUNNING) {
        thou -> registers.rsp -= amount;
        return;
    }
    
    for(unsigned long i = thou -> registers.rsp; amount--; i--) {
        thou -> stack[i] = ((char*)ptr)[amount];
    }
}

void s_processor_stack_top(s_processor* thou, void* ptr, unsigned long amount) {
    if(thou -> registers.rsp < amount) {
        proc_error(thou, PROC_STATE_STACK_UNDERFLOW);
        return;
    }
    
    unsigned long count = thou -> registers.rsp;
    
    for(; amount--; count--) {
        ((char*)ptr)[amount] = thou -> stack[count];
    }
}

void s_processor_stack_pop(s_processor* thou, void* ptr, unsigned long amount) {
    //printf("pop %lu\n", amount);
    if(thou -> registers.rsp < amount) {
        proc_error(thou, PROC_STATE_STACK_UNDERFLOW);
        return;
    }
    
    for(; amount--; thou -> registers.rsp--) {
        ((char*)ptr)[amount] = thou -> stack[thou -> registers.rsp];
    }
    
    s_processor_stack_resize_if_needed(thou);
}


void compiler_previous_char(s_processor* thou) {
    thou -> registers.rip--;
}

unsigned char compiler_next_char(s_processor* thou) {
    unsigned char result = thou -> text[thou -> registers.rip];
    
    if(thou -> registers.rip < thou -> text_length - 1) thou -> registers.rip++;
    else thou -> state = PROC_STATE_REACHED_FILE_END;
    
    return result;
}

void proc_error(s_processor* thou, proc_state error) {
    thou -> state = error;
    
    switch(error) {
        case PROC_STATE_ERROR:
            printf("error: eder has ecountered unexpected error\n");
            break;
        case PROC_STATE_REACHED_FILE_END:
            printf("error: eder has reached file end unexpectedly. Please, make sure you have 'hlt' instruction in the end of your program.\n");
            break;
        case PROC_STATE_INVALID_OPCODE:
            printf("error: eder has encountered an incorrect operation code 0x%X\n", thou -> text[thou -> registers.rip - 1] & 0xFF);
            break;
        case PROC_STATE_OUT_OF_MEMORY:
            printf("error: eder has ran out of memory\n");
            break;
        case PROC_STATE_STACK_UNDERFLOW:
            printf("error: eder has ecountered stack underflow error\n");
            break;
        case PROC_STATE_DIVISION_BY_ZERO:
            printf("error: division by zero\n");
            break;
        case PROC_STATE_MATHEMATICAL_EXCEPTION:
            printf("error: mathematical exception\n");
            break;
        default:
            break;
    }
}

unsigned long long read_integer(s_processor* thou, size_t bytes) {
    unsigned long long result = 0;
    unsigned long long shift = 0;
    
    for(int i = 0; i < bytes; i++) {
        result += (unsigned long long)compiler_next_char(thou) << shift;
        shift += 8;
    }
    
    return result;
}

void parse_instruction(s_processor* thou) {
    const char command = compiler_next_char(thou);
    
    switch (command) {
    case PROC_COMMAND_PUSH:
        proc_push_instruction_handler(thou, false);
        break;
    case PROC_COMMAND_POP:
        proc_pop_instruction_handler(thou);
        break;
    case PROC_COMMAND_ADD:
            proc_math_instruction_handler(thou, 0);
        break;
    case PROC_COMMAND_SUBTRACT:
            proc_math_instruction_handler(thou, 1);
        break;
    case PROC_COMMAND_MULTIPLY:
            proc_math_instruction_handler(thou, 2);
        break;
    case PROC_COMMAND_DIVIDE:
            proc_math_instruction_handler(thou, 3);
        break;
    case PROC_COMMAND_IN:
        proc_in_instruction_handler(thou);
        break;
    case PROC_COMMAND_OUT:
        proc_out_instruction_handler(thou);
        break;
    case PROC_COMMAND_HLT:
        thou -> state = PROC_STATE_HALTED;
        break;
    case PROC_COMMAND_SQRT:
        proc_sqrt_instruction_handler(thou);
        break;
    case PROC_COMMAND_NOP:
            
        break;
            
    case PROC_CMNDXTN_PUSH_REGISTER:
        proc_push_instruction_handler(thou, true);
        break;
    default:
        proc_error(thou, PROC_STATE_INVALID_OPCODE);
        return;
    }
}

void proc_interpret(const char* text, unsigned long length) {
    s_processor thou = {0};
    
    thou.text = text;
    thou.text_length = length;
    thou.state = PROC_STATE_RUNNING;
    thou.stack_capacity = 256;
    thou.stack = calloc(thou.stack_capacity, 1);
    
    // Skip "#!/usr/bin/env eder" line
    
    if(compiler_next_char(&thou) == '#') {
        char c;
        while((c = compiler_next_char(&thou)) != '\n') {
            if(c == '\0') return;
        }
    } else {
        compiler_previous_char(&thou);
    }
    
    uint32_t readen_vendor = (uint32_t)read_integer(&thou, sizeof(uint32_t));
    uint32_t readen_magic = (uint32_t)read_integer(&thou, sizeof(uint32_t));
    
    if(compiler_vendor != readen_vendor) {
        printf("Unable to run this file: this binary file was built with compiler v%d.%d.%d.%d, but the processor is v%d.%d.%d.%d\n",
               (char)(readen_vendor   >> 24), (char)(readen_vendor   >> 16), (char)(readen_vendor)   >> 8, (char)readen_vendor,
               (char)(compiler_vendor >> 24), (char)(compiler_vendor >> 16), (char)(compiler_vendor) >> 8, (char)compiler_vendor);
        
        return;
    }
    
    if(compiler_magic != readen_magic) {
        printf("Unable to run this file: failed magic number check\n");
        return;
    }
    
    while(thou.state == PROC_STATE_RUNNING) {
        parse_instruction(&thou);
    }
}
