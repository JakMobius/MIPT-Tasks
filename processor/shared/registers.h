
#ifndef registers_h
#define registers_h

#include <stdio.h>

typedef struct s_processor_registers {
    unsigned long long rip;
    unsigned long long rsp;
    unsigned long long rap;
    unsigned long long rbp;
    unsigned long long rcp;
    unsigned long long rdp;
} s_processor_registers;

typedef enum s_processor_register_types {
    PROC_REGISTER_RIP,
    PROC_REGISTER_RSP,
    PROC_REGISTER_RAX,
    PROC_REGISTER_RBX,
    PROC_REGISTER_RCX,
    PROC_REGISTER_RDX,
    PROC_REGISTER_EAX,
    PROC_REGISTER_EBX,
    PROC_REGISTER_ECX,
    PROC_REGISTER_EDX,
    PROC_REGISTER_AX,
    PROC_REGISTER_BX,
    PROC_REGISTER_CX,
    PROC_REGISTER_DX,
    PROC_REGISTER_AH,
    PROC_REGISTER_AL,
    PROC_REGISTER_BH,
    PROC_REGISTER_BL,
    PROC_REGISTER_CH,
    PROC_REGISTER_CL,
    PROC_REGISTER_DH,
    PROC_REGISTER_DL
} s_processor_register_types;

extern const size_t PROC_REGISTER_OFFSETS[];
extern const size_t PROC_REGISTER_SIZES[];

#endif /* registers_h */
