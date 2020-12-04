
#include "file_io.h"

file_op_result read_file(const char* filepath, const char** result, unsigned long* length) {
    struct stat filedata = {};

    if(filepath == NULL) {
        return FILE_OP_INVALID_ARGUMENT;
    }

    if(stat(filepath, &filedata) == -1) {
        return FILE_OP_READ_ERROR;
    }

    unsigned long file_length = filedata.st_size + 1;

    char* buffer = (char*) calloc(1, file_length);

    if(!buffer) {
        return FILE_OP_NOT_ENOUGH_MEMORY;
    }

    if(length != NULL) {
        *length = file_length;
    }

    FILE* input = fopen(filepath, "rb");
    size_t readen = fread(buffer, 1, file_length, input);
    buffer[readen] = '\0';
    fclose(input);

    if(result != NULL) {
        *result = buffer;
    }

    return FILE_OP_OK;
}

const char* filename(const char* filepath) {
    char* index = strrchr(filepath, '/');
    if(index == NULL) return filepath;
    return index + 1;
}
