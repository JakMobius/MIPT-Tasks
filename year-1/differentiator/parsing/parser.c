
#include <assert.h>
#include "parser.h"

s_parser* parser_create(s_compiler* compiler) {
    s_parser* parser = calloc(1, sizeof(s_parser));
    if(!parser) return NULL;
    parser_init(parser, compiler);
    return parser;
}

void parser_init(s_parser* parser, s_compiler* compiler) {
    if(!parser) return;

    parser->compiler = compiler;
    parser->position_stack_capacity = 32;
    parser->position_stack_size = 0;
    parser->position_stack = calloc(parser->position_stack_capacity, sizeof(s_parser_position));
}

void parser_destruct(s_parser* parser) {
    if(!parser) return;
    free(parser->position_stack);
}

void parser_release(s_parser* parser) {
    parser_destruct(parser);
    free(parser);
}

void parser_position_stack_resize(s_parser* parser, int new_capacity) {
    s_parser_position* new_stack = realloc(parser->position_stack, new_capacity * sizeof(s_parser_position));
    if(!new_stack) {
        compiler_out_of_memory(parser->compiler);
    }

    parser->position_stack = new_stack;
}

s_tree* parser_parse_file(s_parser* parser, const char* filename, const char* text) {
    parser->position.filename = filename;
    parser->position.index = 0;
    parser->position.ch = 1;
    parser->position.line = 1;
    parser->text = text;

    s_tree_node* node = parse_grammatic_global(parser);
    s_tree* tree = tree_new();
    if(!tree) {
        tree_release_subtree(node);
        return NULL;
    }
    tree->root = node;
    return tree;
}

char parser_next_char(s_parser* parser) {
    return parser->text[parser->position.index];
}

void parser_eat_char(s_parser* parser) {
    char c = parser_next_char(parser);

    assert(c != '\0');

    parser->position.ch++;
    parser->position.index++;
    if(c == '\n') {
        parser->position.line++;
        parser->position.ch = 1;
    }
}

bool parser_save_position(s_parser* parser) {
    if(parser->position_stack_size == parser->position_stack_capacity) {
        parser_position_stack_resize(parser, parser->position_stack_capacity * 2);
        if(parser->compiler->state) return false;
    }

    parser->position_stack[parser->position_stack_size++] = parser->position;

    return true;
}

bool parser_restore_position(s_parser* parser) {
    if(parser->position_stack_size == 0) {
        compiler_fatal_error(parser->compiler, "Position stack underflow");
        return false;
    }
    parser->position_stack_size--;
    return true;
}