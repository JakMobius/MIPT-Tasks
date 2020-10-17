
#ifndef commands_h
#define commands_h

#include <stdlib.h>

typedef enum proc_commands : char {
    PROC_COMMAND_PUSH,
    PROC_COMMAND_POP,
    PROC_COMMAND_ADD,
    PROC_COMMAND_SUBTRACT,
    PROC_COMMAND_MULTIPLY,
    PROC_COMMAND_DIVIDE,
    PROC_COMMAND_SQRT,
    PROC_COMMAND_IN,
    PROC_COMMAND_OUT,
    PROC_COMMAND_HLT,
    PROC_COMMAND_NOP,
    
    // Command extensions are listed below.
    // They have "CMNDXTN" instead of "COMMAND"
    // in their names to indicate these elements
    // does not have their own individual name,
    // flags, they cannot be entered by user
    // in source code directly. They are just
    // extensions for existant command codes.
    
    PROC_CMNDXTN_PUSH_REGISTER,
    PROC_CMNDXTN_IN_REGISTER,
    PROC_CMNDXTN_OUT_REGISTER
} proc_command;

typedef enum proc_flags : char {
    PROC_FLAG_NONE,
    PROC_FLAG_FLOAT64,
    PROC_FLAG_FLOAT32,
    PROC_FLAG_INT64,
    PROC_FLAG_INT32,
    PROC_FLAG_INT16,
    PROC_FLAG_INT8,
    PROC_FLAG_UNSIGNED_INT64,
    PROC_FLAG_UNSIGNED_INT32,
    PROC_FLAG_UNSIGNED_INT16,
    PROC_FLAG_UNSIGNED_INT8
} e_proc_flags;

extern const unsigned long long PROC_FLAG_MASK_FLOATS;
extern const unsigned long long PROC_FLAG_MASK_SIGNED_INTS;
extern const unsigned long long PROC_FLAG_MASK_UNSIGNED_INTS;
extern const unsigned long long PROC_FLAG_MASK_INTS;
extern const unsigned long long PROC_FLAG_MASK_DECIMALS;
extern const unsigned long long PROC_COMMAND_FLAGS[];
extern const size_t PROC_FLAG_SIZES[];
extern const uint64_t compiler_vendor;
extern const uint32_t compiler_magic;

#endif /* commands_h */
