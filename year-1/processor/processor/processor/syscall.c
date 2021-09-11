

#include "syscall.h"

void proc_perform_syscall(s_processor* thou) {
    unsigned char syscall_index;
    s_processor_stack_pop(thou, &syscall_index, 1);
    if(thou->state) return;
    
    switch(syscall_index) {
        case PROC_SYSCALL_INSTRUCTION_AMOUNT:
            s_processor_stack_push(thou, &thou->instructions_executed, sizeof(unsigned long long));
            break;
        case PROC_SYSCALL_CLOCK_TIME:
            {
                double seconds = (double)clock() / (double)CLOCKS_PER_SEC;
                s_processor_stack_push(thou, &seconds, sizeof(double));
            }
            break;
        case PROC_SYSCALL_PUTC:
            printf("\n");
            break;
    }
}
