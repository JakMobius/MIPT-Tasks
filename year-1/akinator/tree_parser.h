

#ifndef AKINATOR_TREE_PARSER_H
#define AKINATOR_TREE_PARSER_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tree.h"

typedef uint16_t tree_parser_char_map;

typedef struct tree_parser {
    const char* text;
    s_tree* target;
    unsigned long text_index;
} s_tree_parser;

e_tree_error tree_parser_construct(s_tree_parser* thou);

void tree_parser_destruct(s_tree_parser* thou);

e_tree_error tree_parser_read_subtree(s_tree_parser* parser, s_tree_node** node);

e_tree_error tree_parser_read_tree(s_tree* target, const char* text);

char tree_parser_next_char(s_tree_parser* parser);
void tree_parser_previous_char(s_tree_parser* parser);
char tree_parser_char_ahead(s_tree_parser* parser);

char* tree_parser_read_string_literal(s_tree_parser* parser);
void tree_parser_read_until_char(s_tree_parser* parser, char expect);

e_tree_error tree_parser_serialize_subtree(s_tree_node* target, FILE* file, int offset);
e_tree_error tree_parser_serialize(s_tree* target, FILE* file);

e_tree_error tree_parser_dump_subtree(s_tree_node* target, FILE* file, int* node_counter);
e_tree_error tree_parser_dump(s_tree* target, FILE* file);

#endif //AKINATOR_TREE_PARSER_H
