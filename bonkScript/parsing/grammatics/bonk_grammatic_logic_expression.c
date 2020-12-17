
#include "bonk_grammatic_logic_expression.h"

s_tree_node* bonk_parse_grammatic_logic_expression(s_bonk_parser* parser) {
    s_tree_node* term = bonk_parse_grammatic_logic_term(parser);
    s_tree_node* result = term;

    do {
        s_bonk_lexeme* next = bonk_parser_next_lexeme(parser);
        if(next->type != BONK_LEXEME_KEYWORD) break;
        e_bonk_operator_type operator_type = 0;

        switch(next->keyword_data.keyword_type) {
            case BONK_KEYWORD_AND: operator_type = BONK_OPERATOR_AND; break;
            case BONK_KEYWORD_OR: operator_type = BONK_OPERATOR_OR; break;
            default: return result;
        }

        bonk_parser_eat_lexeme(parser);

        s_tree_node* next_term = bonk_parse_grammatic_expression(parser);
        if(!next_term) {
            if(!parser->compiler->state) {
                bonk_parser_error(parser, "expected expression");
            }
            return NULL;
        }

        s_tree_node* sum = (s_tree_node*) tree_create_node_operator(operator_type);
        if(!sum) {
            compiler_out_of_memory(parser->compiler);
            return NULL;
        }
        sum->left = result;
        sum->right = next_term;
        result = sum;

    } while(true);

    return result;
}