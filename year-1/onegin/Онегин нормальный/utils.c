
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

size_t strchrcnt(const char* buffer, char c, size_t length) {
    size_t line_num = 0;
    
    for(size_t i = 0; i < length; i++) {
        if(buffer[i] == c) {
            line_num++;
        }
    }
    
    return line_num;
}
