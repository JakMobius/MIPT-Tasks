
#include "file_io.h"

const char* read_file(const char* filepath, file_op_result* result, unsigned long* length) {
    struct stat filedata = {};
    
    if(filepath == NULL) {
        if(result != NULL) {
            *result = FILE_OP_INVALID_ARGUMENT;
        }
        return NULL;
    }
    
    if(stat(filepath, &filedata) == -1) {
        if(result != NULL) {
            *result = FILE_OP_READ_ERROR;
        }
        return NULL;
    }
    
    unsigned long file_length = filedata.st_size + 1;
    
    char* buffer = (char*) malloc(file_length);
    
    if(!buffer) {
        if(result != NULL) {
            *result = FILE_OP_NOT_ENOUGH_MEMORY;
        }
        return NULL;
    }
    
    if(length != NULL) {
        *length = file_length;
    }
    
    FILE* input = fopen(filepath, "rb");
    size_t readen = fread(buffer, 1, file_length, input);
    buffer[readen] = '\0';
    fclose(input);
    
    if(result != NULL) {
        *result = FILE_OP_OK;
    }
    
    return buffer;
}

const char* filename(const char* filepath) {
    char* index = strrchr(filepath, '/');
    if(index == NULL) return filepath;
    return index + 1;
}
