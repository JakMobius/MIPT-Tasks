
#include "bonk_parser.h"

s_bonk_parser* bonk_parser_create(s_bonk_compiler* compiler) {
    s_bonk_parser* parser = calloc(1, sizeof(s_bonk_parser));
    if(!parser) return NULL;
    bonk_parser_init(parser, compiler);
    return parser;
}

void bonk_parser_init(s_bonk_parser* parser, s_bonk_compiler* compiler) {
    if(!parser) return;

    parser->compiler = compiler;
    parser->position_stack_capacity = 32;
    parser->position_stack_size = 0;
    parser->position_stack = calloc(parser->position_stack_capacity, sizeof(*parser->position_stack));
}

void bonk_parser_destruct(s_bonk_parser* parser) {
    if(!parser) return;
    free(parser->position_stack);
}

void bonk_parser_release(s_bonk_parser* parser) {
    bonk_parser_destruct(parser);
    free(parser);
}

void bonk_parser_position_stack_resize(s_bonk_parser* parser, int new_capacity) {
    unsigned long* new_stack = realloc(parser->position_stack, new_capacity * sizeof(*parser->position_stack));
    if(!new_stack) {
        compiler_out_of_memory(parser->compiler);
    }

    parser->position_stack = new_stack;
}

s_tree* bonk_parser_parse_file(s_bonk_parser* parser) {
    parser->input = parser->compiler->lexical_analyzer->lexemes;

    s_tree_node* node = bonk_parse_grammatic_global(parser);
    s_tree* tree = tree_new();
    if(!tree) {
        tree_release_subtree(node);
        return NULL;
    }
    tree->root = node;
    return tree;
}

void bonk_parser_spit_lexeme(s_bonk_parser* parser) {
    assert(parser->position > 0);
    parser->position--;
}

s_bonk_lexeme* bonk_parser_next_lexeme(s_bonk_parser* parser) {
    return parser->input + parser->position;
}

void bonk_parser_eat_lexeme(s_bonk_parser* parser) {
    s_bonk_lexeme* c = bonk_parser_next_lexeme(parser);
    assert(c->type);
    parser->position++;
}

bool bonk_parser_save_position(s_bonk_parser* parser) {
    if(parser->position_stack_size == parser->position_stack_capacity) {
        bonk_parser_position_stack_resize(parser, parser->position_stack_capacity * 2);
        if(parser->compiler->state) return false;
    }

    parser->position_stack[parser->position_stack_size++] = parser->position;

    return true;
}

bool bonk_parser_restore_position(s_bonk_parser* parser) {
    if(parser->position_stack_size == 0) {
        bonk_compiler_fatal_error(parser->compiler, "Position stack underflow");
        return false;
    }
    parser->position_stack_size--;
    return true;
}