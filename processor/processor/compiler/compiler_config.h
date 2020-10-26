
#ifndef compiler_config_h
#define compiler_config_h

#include <stdbool.h>
#include <stdio.h>

typedef struct s_compiler_config {
    bool insert_line_flags;
    bool put_shebang;
    bool list_line_flags;
    const char* file;
    const char* full_path;
    FILE* error_file;
    FILE* listing_file;
    FILE* output_file;
} s_compiler_config;

#endif /* compiler_config_h */
