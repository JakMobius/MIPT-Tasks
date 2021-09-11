
#ifndef proc_syscall_h
#define proc_syscall_h

typedef enum proc_syscall {
    PROC_SYSCALL_INSTRUCTION_AMOUNT,
    PROC_SYSCALL_CLOCK_TIME,
    PROC_SYSCALL_PUTC
} e_proc_syscall;

#include <stdio.h>
#include <time.h>
#include "processor.h"

void proc_perform_syscall(s_processor* thou);

#endif /* proc_syscall_h */
