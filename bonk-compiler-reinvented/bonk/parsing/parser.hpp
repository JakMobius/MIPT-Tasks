

#ifndef BONKSCRIPT_BONK_PARSER_H
#define BONKSCRIPT_BONK_PARSER_H

#include <stdlib.h>
#include <stdbool.h>
#include <cassert>

namespace bonk {

struct parser;

}

#include "../compiler.hpp"
#include "./lexic/lexical_analyzer.hpp"
#include "./grammatics/grammatic_global.hpp"
#include "../../utils/dynamic_array.cpp"
#include "../../utils/default_alloc_construct.hpp"

namespace bonk {

struct parser {
    unsigned long position;
    dynamic_array<lexeme>* input;
    compiler* linked_compiler;

    void warning(const char *format, ...);
    void error(const char *format, ...);
    void fatal_error(const char *format, ...);

    lexeme* next_lexeme();
    void eat_lexeme();
    void spit_lexeme();

    static parser* create(compiler* compiler);
    void construct(compiler* compiler);
    void destruct();
    void release();

    tree_node_list<tree_node*>* parse_file(dynamic_array<lexeme>* lexemes);
    bool append_file(dynamic_array<lexeme>* lexemes, tree_node_list<tree_node*>* target);
};

}

#endif //BONKSCRIPT_BONK_PARSER_H
