
#include "bonk_grammatic_logic_term.h"

s_tree_node* bonk_parse_grammatic_logic_term(s_bonk_parser* parser) {

    s_tree_node* expression = bonk_parse_grammatic_unary_operator(parser);
    if(expression) return expression;
    if(parser->compiler->state) return NULL;

    expression = bonk_parse_grammatic_assignment(parser);
    if(expression) return expression;
    if(parser->compiler->state) return NULL;

    expression = bonk_parse_grammatic_comparation(parser);
    if(expression) return expression;
    if(parser->compiler->state) return NULL;

    return expression;
}
