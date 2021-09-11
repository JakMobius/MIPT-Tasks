
#ifndef commands_h
#define commands_h

#include <stdlib.h>

typedef enum proc_command {
#define ADD_COMMAND(enum_name, command_name, command_flags, compile, process, decompile) enum_name,
#include "../shared/command_pool.h"

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
extern const unsigned long long PROC_FLAG_MASK_UNSIGNED_INTS;
extern const unsigned long long PROC_FLAG_MASK_SIGNED_INTS;
extern const unsigned long long PROC_FLAG_MASK_INTS;
extern const unsigned long long PROC_FLAG_MASK_DECIMALS;
extern const unsigned long long PROC_FLAG_MASK_NONE;
extern const unsigned long long PROC_COMMAND_FLAGS[];
extern const size_t PROC_FLAG_SIZES[];
extern const uint64_t compiler_vendor;
extern const uint32_t compiler_magic;

#endif /* commands_h */
