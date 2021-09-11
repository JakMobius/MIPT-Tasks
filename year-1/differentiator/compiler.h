#ifndef BONKSCRIPT_COMPILER_H
#define BONKSCRIPT_COMPILER_H

#include <stdio.h>
#include "parsing/parser.h"
#include "errors.h"
#include "simplifier.h"

typedef struct parser s_parser;

typedef struct compiler_config {
    FILE* error_file;
} s_compiler_config;

typedef enum compiler_state {
    COMPILER_OK,
    COMPILER_STATE_ERROR,
    COMPILER_STATE_FATAL_ERROR
} e_compiler_state;

typedef struct compiler {
    s_compiler_config* config;
    s_parser* parser;
    e_compiler_state state;
} s_compiler;

void compiler_out_of_memory(s_compiler* compiler);
s_compiler* compiler_create(s_compiler_config* config);
void compiler_construct(s_compiler* compiler, s_compiler_config* config);
void complier_destruct(s_compiler* compiler);
void compiler_release(s_compiler* compiler);
void compiler_subtree_compile(s_compiler* compiler, s_tree_node** node_link);
void compiler_compile(s_compiler* compiler, s_tree* expression);
int tree_variable_by_string(const char* string, unsigned long length);
e_tree_function tree_function_by_string(const char* string, unsigned long length);

#endif //BONKSCRIPT_COMPILER_H