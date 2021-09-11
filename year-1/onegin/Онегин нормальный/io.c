
#include "io.h"
#include "sorting.h"

s_text* read_file(const char* filepath) {
    struct stat filedata = {};
    if(stat(filepath, &filedata) == -1) {
        perror("Unable to read file");
        return NULL;
    }
    long long length = filedata.st_size;
    
    char* buffer = (char*) malloc(length);
    
    if(!buffer) {
        return NULL;
    }
    
    if(!buffer) {
        OUT_OF_MEMORY_MESSAGE;
        return NULL;
    }
    
    FILE* input = fopen(filepath, "rb");
    fread(buffer, 1, length, input);
    fclose(input);
    
    s_text* text = s_text_create(buffer, length);
    
    if(!text) {
        free(buffer);
    }
    
    return text;
}

void write_file(FILE* file, s_text* text) {
    for(size_t i = 0; i < text -> line_count; i++) {
        size_t length = text -> line_map[i] -> string_length;
        
        fwrite(text -> line_map[i] -> string, 1, length, file);
    }
}
