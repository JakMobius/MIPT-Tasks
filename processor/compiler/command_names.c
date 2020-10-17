
#include "command_names.h"

const long COMPILER_MAX_COMMAND_LENGTH = 4;
const long COMPILER_MAX_FLAG_LENGTH = 2;

const char* COMPILER_COMMAND_NAMES[] = {
    "push", // PROC_COMMAND_PUSH
    "pop",  // PROC_COMMAND_POP
    "add",  // PROC_COMMAND_ADD
    "sub",  // PROC_COMMAND_SUBTRACT
    "mul",  // PROC_COMMAND_MULTIPLY
    "div",  // PROC_COMMAND_DIVIDE
    "sqrt", // PROC_COMMAND_SQRT
    "in",   // PROC_COMMAND_IN
    "out",  // PROC_COMMAND_OUT
    "hlt",  // PROC_COMMAND_HLT
    "nop",  // PROC_COMMAND_NOP
    NULL
};

const char* COMPILER_FLAG_NAMES[] = {
    "",   // PROC_FLAG_NONE            // cannot be entered in source code file
    "d",  // PROC_FLAG_FLOAT64,        // d  = double
    "f",  // PROC_FLAG_FLOAT32,        // f  = float
    "l",  // PROC_FLAG_INT64,          // l  = long
    "i",  // PROC_FLAG_INT32,          // i  = int
    "s",  // PROC_FLAG_INT16,          // s  = short
    "b",  // PROC_FLAG_INT8,           // b  = byte
    "ul", // PROC_FLAG_UNSIGNED_INT64, // ul = unsigned long
    "ui", // PROC_FLAG_UNSIGNED_INT32, // ui = unsigned int
    "us", // PROC_FLAG_UNSIGNED_INT16, // us = unsigned short
    "ub", // PROC_FLAG_UNSIGNED_INT8,  // ub = unsigned byte
    NULL
};
