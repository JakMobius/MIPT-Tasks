
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef istack_pointer_check_h
#ifndef ISTACK_DECLARATIONS_ALLOWED
#error this file cannot be included directly, use #include "stack.h" instead.
#else
#define istack_pointer_check_h

static bool istack_signal_received;
static uint64_t istack_instruction_pointer;
static struct sigaction istack_stored_sigaction_segv;
static struct sigaction istack_stored_sigaction_bus;

void istack_signal_handler(int nSignum, siginfo_t* si, void* vcontext) {
    const ucontext_t *con = (ucontext_t *)vcontext;
    istack_signal_received = true;
    con -> uc_mcontext -> __ss.__rip = istack_instruction_pointer;
}

void istack_setup_signal_handling() {
    sigaction(SIGSEGV, NULL, &istack_stored_sigaction_segv);
    sigaction(SIGBUS, NULL, &istack_stored_sigaction_bus);
    
    struct sigaction action = {};
    
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = istack_signal_handler;
    sigaction(SIGSEGV, &action, NULL);
    sigaction(SIGBUS, &action, NULL);
}

void istack_restore_signal_handling() {
    sigaction(SIGSEGV, &istack_stored_sigaction_segv, NULL);
    sigaction(SIGBUS, &istack_stored_sigaction_bus, NULL);
}

bool istack_pointer_valid(void* addr, int size) {
    if ((uint64_t)addr < 4096 || (uint64_t)addr % (uint64_t)size != 0) {
        return false; // Either alignment is not valid or addr targets to first memory page
    }
    istack_signal_received = false;
    
    asm volatile("1: lea 1b(%%rip), %0;": "=a"(istack_instruction_pointer));
    
    if(istack_signal_received) {
        return false;
    }
    
    volatile char __attribute__((unused)) st = *(char*)addr;

    return true;
}

#endif
#endif
