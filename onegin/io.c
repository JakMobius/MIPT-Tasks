
#include "io.h"

char* read_file(const char* filepath, long long* length) {
    struct stat filedata;
    if(stat(filepath, &filedata) == -1) {
        return NULL;
    }
    *length = filedata.st_size;
    
    char* buffer = (char*) malloc(*length);
    
    CHECK_POINTER(buffer);
    
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
