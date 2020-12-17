
#include "../shared/commands.h"
#include "../compiler/command_names.h"
#include "../compiler/register_names.h"
#include "processor.h"
#include "instruction_handlers.h"
#include "videomemory.h"

void s_processor_stack_resize(s_processor* thou, int direction) {
    
    unsigned long capacity = thou -> stack_capacity;
    
    if(direction > 0) capacity *= 2;
    else capacity /= 2;
    
    if(capacity < thou -> registers.rsp) capacity = thou -> registers.rsp;
    
    if(capacity > 32768) {
        proc_error(thou, PROC_STATE_OUT_OF_MEMORY);
        return;
    }
    
    if(capacity < 256) capacity = 256;
    if(capacity == thou -> stack_capacity) return;
    
    char* buffer = (char*)realloc(thou -> stack, capacity);
    
    if(buffer == NULL) {
        proc_error(thou, PROC_STATE_OUT_OF_MEMORY);
        return;
    }
    
    thou -> stack_capacity = capacity;
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
    thou -> registers.rsp += amount;
    if(thou->state) return;
    
    s_processor_stack_resize_if_needed(thou);
    
    if(thou -> state != PROC_STATE_RUNNING) {
        thou -> registers.rsp -= amount;
        return;
    }
    
    for(unsigned long i = thou -> registers.rsp - 1; amount--; i--) {
        thou -> stack[i] = ((char*)ptr)[amount];
    }
}

void s_processor_stack_top(s_processor* thou, void* ptr, unsigned long amount) {
    s_processor_stack_resize_if_needed(thou);
    if(thou->state) return;
    
    if(thou -> registers.rsp < amount) {
        proc_error(thou, PROC_STATE_STACK_UNDERFLOW);
        return;
    }
    
    unsigned long count = thou -> registers.rsp - 1;
    
    for(; amount--; count--) {
        ((char*)ptr)[amount] = thou -> stack[count];
    }
}

void s_processor_stack_pop(s_processor* thou, void* ptr, unsigned long amount) {
    s_processor_stack_resize_if_needed(thou);
    if(thou->state) return;
    
    if(thou -> registers.rsp < amount) {
        proc_error(thou, PROC_STATE_STACK_UNDERFLOW);
        return;
    }
    
    unsigned long long old_rsp = thou->registers.rsp;
    thou->registers.rsp -= amount;
    
    for(; amount--;) {
        ((char*)ptr)[amount] = thou->stack[--old_rsp];
    }
    
    s_processor_stack_resize_if_needed(thou);
}


void compiler_previous_char(s_processor* thou) {
    thou -> registers.rip--;
}

unsigned char compiler_next_char(s_processor* thou) {
    if(thou->registers.rip >= thou->text_length) {
        proc_error(thou, PROC_STATE_REACHED_FILE_END);
        return 0;
    }
    
    unsigned char result = thou -> text[thou -> registers.rip];
    
    thou -> registers.rip++;
    
    return result;
}

void proc_error(s_processor* thou, proc_state error) {
    thou -> state = error;
    
    switch(error) {
        case PROC_STATE_RUNNING:
            return;
        case PROC_STATE_HALTED:
            return;
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
        case PROC_ERROR_WINDOW_ALREADY_OPEN:
            printf("error: window is already opened\n");
            break;
        case PROC_ERROR_NO_WINDOW:
            printf("error: no active window\n");
            break;
    }
    
    proc_dump(thou);
}

void proc_dump(s_processor* thou) {
    printf("Core dump:\n");
    printf("- Line: ");
    
    if(thou -> debug_line == -1) printf("unknown. Recompile with '--debug-lines' flag\n");
    else printf("%d\n", thou -> debug_line);
    printf("- Register state: \n");
    
    for(int i = 0; PROC_REGISTER_SIZES[i]; i++) {
        printf(" - %s: ", COMPILER_REGISTER_NAMES[i]);
        
        unsigned long long register_value = 0;
        
        char* register_pointer = (char*)(&thou -> registers);
        
        memcpy(&register_value, register_pointer + PROC_REGISTER_OFFSETS[i], PROC_REGISTER_SIZES[i]);
        
        if     (PROC_REGISTER_SIZES[i] == 8) printf("%016llx", register_value);
        else if(PROC_REGISTER_SIZES[i] == 4) printf("%08llx",  register_value);
        else if(PROC_REGISTER_SIZES[i] == 2) printf("%04llx",  register_value);
        else if(PROC_REGISTER_SIZES[i] == 1)  printf("%02llx",  register_value);
        
        if(PROC_REGISTER_SIZES[i] == sizeof(double)) {
            printf(" double = %lg", *((double*)&register_value));
        } else if(PROC_REGISTER_SIZES[i] == sizeof(float)) {
            printf(" float = %f", *((float*)&register_value));
        }
        
        printf("\n");
    }
    
    printf("- Stack bytes: \n");
    
    int limit = 32;
    
    for(int i = (int)thou -> registers.rsp - 1; i >= 0 && limit > 0; i--, limit--) {
        printf(" - [0x%04x] = 0x%02x\n", i, limit);
    }
}

unsigned long long proc_read_long_long(s_processor* thou) {
    
    return proc_read_integer(thou, 8);
    
    // this is totally unsafe
//    if(thou->text_length < thou->registers.rsp + 8) {
//        proc_error(thou, PROC_STATE_REACHED_FILE_END);
//        return 0;
//    }
//    unsigned long long result = *(unsigned long long*)(thou->text + thou->registers.rip);
//
//    thou->registers.rip += 8;
//
//    return result;
}

unsigned long long proc_read_integer(s_processor* thou, size_t bytes) {
    unsigned long long result = 0;
    unsigned long long shift = 0;
    
    for(int i = 0; i < bytes; i++) {
        result += (unsigned long long)compiler_next_char(thou) << shift;
        shift += 8;
    }
    
    return result;
}

void proc_parse_instruction(s_processor* thou) {
    proc_command command = compiler_next_char(thou);
    
    //printf("%016llx, %s\n", thou -> registers.rip - 1, COMPILER_COMMAND_NAMES[command]);
    
    if(thou -> state != PROC_STATE_RUNNING) return;
    
    switch (command) {
        
#define ADD_COMMAND(enum_name, command_name, command_flags, compile, process, decompile)    \
                                                                                            \
        case enum_name:                                                                     \
            process                                                                         \
            break;
            
#include "../shared/command_pool.h"
#undef ADD_COMMAND
            
        default:
            proc_error(thou, PROC_STATE_INVALID_OPCODE);
            return;
    }
}

void proc_interpret(const char* text, unsigned long length) {
    s_processor thou = {0};
    
    const unsigned long memory_amount = 640 * 1024; // Хватит каждому
    
    thou.text = text;
    thou.text_length = length;
    thou.state = PROC_STATE_RUNNING;
    thou.stack_capacity = 256;
    thou.stack = calloc(thou.stack_capacity, 1);
    thou.debug_line = -1;
    thou.memory = malloc(memory_amount);
    thou.video_memory = proc_vm_create();
    
    // TODO: некрасиво.
    
    thou.video_memory_offset = 0x80000;
    
    // Skip "#!/usr/bin/env eder" line
    
    if(compiler_next_char(&thou) == '#') {
        char c;
        while((c = compiler_next_char(&thou)) != '\n') {
            if(c == '\0') return;
        }
    } else {
        compiler_previous_char(&thou);
    }
    
    uint32_t readen_vendor = (uint32_t)proc_read_integer(&thou, sizeof(uint32_t));
    uint32_t readen_magic = (uint32_t)proc_read_integer(&thou, sizeof(uint32_t));
    
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
        proc_parse_instruction(&thou);
    }
    
    proc_vm_destroy(thou.video_memory);
}
