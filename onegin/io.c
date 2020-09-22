
#include "io.h"

char* read_file(const char* filepath, long long* length) {
    struct stat filedata = {};
    if(stat(filepath, &filedata) == -1) {
        perror("Unable to read file");
        return NULL;
    }
    *length = filedata.st_size;
    
    char* buffer = (char*) malloc(*length);
    
    if(!buffer) {
        OUT_OF_MEMORY_MESSAGE;
        return NULL;
    }
    
    FILE* input = fopen(filepath, "rb");
    fread(buffer, 1, *length, input);
    fclose(input);
    
    return buffer;
}

void write_file(const char* filepath, char* buffer) {
    FILE* output = fopen(filepath, "w");
    
    if(output == NULL) {
        perror("Unable to write to file");
        return;
    }
    
    fprintf(output, "%s", buffer);
    fclose(output);
}
