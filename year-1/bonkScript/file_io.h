
#ifndef file_io_h
#define file_io_h

#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

typedef enum file_op_result {
    FILE_OP_READ_ERROR,
    FILE_OP_NOT_ENOUGH_MEMORY,
    FILE_OP_INVALID_ARGUMENT,
    FILE_OP_OK
} file_op_result;

const char* read_file(const char* filepath, file_op_result* result, unsigned long* length);

#endif /* file_io_h */
