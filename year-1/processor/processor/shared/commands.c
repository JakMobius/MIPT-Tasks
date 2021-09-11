
#include "commands.h"

const uint64_t compiler_vendor = 0x00010000; // v1.0
const uint32_t compiler_magic = 'rede';

const unsigned long long PROC_FLAG_MASK_FLOATS =
    (1 << PROC_FLAG_FLOAT64) |
    (1 << PROC_FLAG_FLOAT32);

const unsigned long long PROC_FLAG_MASK_SIGNED_INTS =
    (1 << PROC_FLAG_INT64) |
    (1 << PROC_FLAG_INT32) |
    (1 << PROC_FLAG_INT16) |
    (1 << PROC_FLAG_INT8);

const unsigned long long PROC_FLAG_MASK_UNSIGNED_INTS =
    (1 << PROC_FLAG_UNSIGNED_INT64) |
    (1 << PROC_FLAG_UNSIGNED_INT32) |
    (1 << PROC_FLAG_UNSIGNED_INT16) |
    (1 << PROC_FLAG_UNSIGNED_INT8);

const unsigned long long PROC_FLAG_MASK_EXTENDABLE_INTS =
    (1 << PROC_FLAG_INT8) |
    (1 << PROC_FLAG_INT16) |
    (1 << PROC_FLAG_INT32);

const unsigned long long PROC_FLAG_MASK_INTS =
    PROC_FLAG_MASK_SIGNED_INTS |
    PROC_FLAG_MASK_UNSIGNED_INTS;

const unsigned long long PROC_FLAG_MASK_DECIMALS =
    PROC_FLAG_MASK_INTS |
    PROC_FLAG_MASK_FLOATS;

const unsigned long long PROC_FLAG_MASK_NONE =
    (1 << PROC_FLAG_NONE);

const unsigned long long PROC_COMMAND_FLAGS[] = {
#define ADD_COMMAND(enum_name, command_name, command_flags, compile, process, decompile) command_flags,
#include "../shared/command_pool.h"
#undef ADD_COMMAND
    0
};

const size_t PROC_FLAG_SIZES[] = {
    0, // PROC_FLAG_NONE
    8, // PROC_FLAG_FLOAT64
    4, // PROC_FLAG_FLOAT32
    8, // PROC_FLAG_INT64
    4, // PROC_FLAG_INT32
    2, // PROC_FLAG_INT16
    1, // PROC_FLAG_INT8
    8, // PROC_FLAG_UNSIGNED_INT64
    4, // PROC_FLAG_UNSIGNED_INT32
    2, // PROC_FLAG_UNSIGNED_INT16
    1  // PROC_FLAG_UNSIGNED_INT8
};
