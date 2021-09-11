#ifndef BONKSCRIPT_BONK_COMPILER_H
#define BONKSCRIPT_BONK_COMPILER_H

namespace bonk {

struct compiler;
struct compiler_config;

enum compiler_state {
    BONK_COMPILER_OK,
    BONK_COMPILER_STATE_ERROR,
    BONK_COMPILER_STATE_FATAL_ERROR
};

}

#include <stdio.h>
#include "../utils/default_alloc_construct.hpp"
#include "parsing/parser.hpp"
#include "parsing/lexic/lexical_analyzer.hpp"
#include "errors.hpp"
#include "tree/nodes/nodes.hpp"
#include "backend/backend.hpp"

namespace bonk {

struct compiler_config {
    FILE* error_file;
    FILE* listing_file;
    backend* compile_backend;
};

struct compiler {
    compiler_config* config;

    parser* parser;
    lexical_analyzer* lexical_analyzer;

    compiler_state state;

    void out_of_memory();
    static compiler* create(compiler_config* config);
    void construct(compiler_config* config);
    void destruct();
    void release();

    void fatal_error_positioned(parser_position* pPosition, const char* string, ...);
    void error_positioned(parser_position* pPosition, const char* string, ...);
    void error(const char *format, ...);
    void warning_positioned(parser_position* pos, const char *format, ...);
    void warning(const char *format, ...);
    void fatal_error(const char *format, ...);

    tree_node_list<tree_node*>* get_ast_of_file_at_path(const char* file_path);

    bool compile_ast(tree_node_list<tree_node*>* ast, FILE* target);
};

}

#endif //BONKSCRIPT_BONK_COMPILER_H