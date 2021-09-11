#ifndef BONKSCRIPT_BONK_COMPILER_H
#define BONKSCRIPT_BONK_COMPILER_H

typedef struct bonk_parser s_bonk_parser;
typedef struct bonk_compiler s_bonk_compiler;
typedef struct bonk_variable s_bonk_variable;

typedef enum bonk_compiler_state {
    BONK_COMPILER_OK,
    BONK_COMPILER_STATE_ERROR,
    BONK_COMPILER_STATE_FATAL_ERROR
} e_bonk_compiler_state;

#include <stdio.h>
#include "bonk_field_list.h"
#include "bonk_scope_stack.h"
#include "parsing/bonk_parser.h"
#include "parsing/lexic/bonk_lexical_analyzer.h"
#include "bonk_variable.h"
#include "bonk_field_list.h"
#include "errors.h"

typedef struct bonk_compiler_config {
    FILE* error_file;
    FILE* target_file;
} s_bonk_compiler_config;

typedef struct bonk_compiler {
    s_bonk_compiler_config* config;

    s_bonk_parser* parser;
    s_bonk_lexical_analyzer* lexical_analyzer;

    e_bonk_compiler_state state;
    s_bonk_scope_stack* scope_stack;
    unsigned long processor_stack_usage;
    unsigned long labels;
    unsigned long cycle_label_id;
    unsigned long scope_stack_offset;

    const char** compiled_files;
    unsigned long compiled_files_amount;
    unsigned long compiled_files_buffer_length;
} s_bonk_compiler;

void compiler_out_of_memory(s_bonk_compiler* compiler);
s_bonk_compiler* bonk_compiler_create(s_bonk_compiler_config* config);
void bonk_compiler_construct(s_bonk_compiler* compiler, s_bonk_compiler_config* config);
void bonk_compiler_destruct(s_bonk_compiler* compiler);
void bonk_compiler_release(s_bonk_compiler* compiler);

void bonk_field_list_declare_variable(s_bonk_compiler* compiler, s_tree_node* node);
void bonk_field_list_declare_block(s_bonk_compiler* compiler, s_tree_node* node);

void bonk_compiler_error_already_defined(s_bonk_compiler* compiler, s_tree_node_identifier* identifier);
void bonk_compiler_error_undefined_reference(s_bonk_compiler* compiler, s_tree_node_identifier* node);

const char* bonk_compiler_get_comparsion_instruction(e_bonk_operator_type operator_type);
const char* bonk_compiler_get_comparsion_instruction_negated(e_bonk_operator_type operator_type);

void bonk_compiler_reference_variable(s_bonk_compiler* compiler, s_tree_node_identifier* identifier);
void bonk_compile_term(s_bonk_compiler* compiler, s_tree_node* node);
void bonk_compile_math_expression(s_bonk_compiler* compiler, s_tree_node* node);
void bonk_compile_factor(s_bonk_compiler* compiler, s_tree_node* node);
void bonk_compile_math_comparsion(s_bonk_compiler* compiler, s_tree_node* node);
void bonk_compile_expression(s_bonk_compiler* compiler, s_tree_node* node, unsigned long stack_bytes_expected);

void bonk_compile_block(s_bonk_compiler* compiler, s_tree_node* node, bool reset_frame_offset);
void bonk_compile_callable_block(s_bonk_compiler* compiler, s_tree_node* node);

unsigned long bonk_compiler_next_label_id(s_bonk_compiler* compiler);

void bonk_compile_program(s_bonk_compiler* compiler, s_tree_node* node);
void bonk_compiler_reference_variable(s_bonk_compiler* compiler, s_tree_node_identifier* identifier);

bool bonk_compiler_resize_compiled_files_buffer(s_bonk_compiler* compiler, unsigned long new_capacity);
bool bonk_compiler_add_compiled_file(s_bonk_compiler* compiler, const char* file_path);
bool bonk_compiler_file_already_compiled(s_bonk_compiler* compiler, const char* file_path);

#endif //BONKSCRIPT_BONK_COMPILER_H