
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

file_op_result read_file(const char* filepath, const char** result, unsigned long* length);
const char* filename(const char* filepath);

#endif /* file_io_h */
