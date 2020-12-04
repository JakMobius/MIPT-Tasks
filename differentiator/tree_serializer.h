

#ifndef AKINATOR_TREE_PARSER_H
#define AKINATOR_TREE_PARSER_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tree.h"
#include <ctype.h>
#include "simplifier.h"

e_tree_error tree_parser_serialize_subtree(s_tree_node* target, FILE* file);
e_tree_error tree_parser_serialize(s_tree* target, FILE* file);

e_tree_error latex_dump_subexpression(s_tree_node* target, FILE* file, s_tree_node_operator* parent);
void latex_add_image(const char* name, FILE* file);
void latex_document_begin(FILE* file);
void latex_expression_begin(FILE* file);
void latex_document_add_expression(s_tree* expression, FILE* file);
void latex_expression_break(FILE* file);
void latex_expression_end(FILE* file);
void latex_document_end(FILE* file);

#endif //AKINATOR_TREE_PARSER_H
