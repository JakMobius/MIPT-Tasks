
#include "utils.h"

void memory_failure(int line, const char* file) {
    fprintf(stderr, "Failed to allocate memory at %s:%d\n", file, line);
    exit(-1);
}

size_t min(size_t a, size_t b) {
    return a < b ? a : b;
}

size_t max(size_t a, size_t b) {
    return a > b ? a : b;
}

size_t replace_chars(char* buffer, char from, char to) {
    size_t line_num = 0;
    
    for(size_t i = 0; buffer[i]; i++) {
        if(buffer[i] == '\n') {
            line_num++;
            buffer[i] = '\0';
        }
    }
    
    return line_num;
}
