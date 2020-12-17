
#include "bonk_grammatic_unary_operator.h"

s_tree_node* bonk_parse_grammatic_bams(s_bonk_parser* parser) {
    s_bonk_lexeme* lexeme = bonk_parser_next_lexeme(parser);
    if(lexeme->type == BONK_LEXEME_INLINE_BAMS) {
        bonk_parser_eat_lexeme(parser);

        s_tree_node* node = (s_tree_node*) tree_create_node_operator(BONK_OPERATOR_BAMS);

        if(!node) {
            compiler_out_of_memory(parser->compiler);
            return NULL;
        }

        node->source_position = bonk_parser_position_clone(lexeme->position);
        node->left = (s_tree_node*) lexeme->identifier_data.identifier;
        return node;
    }

    bonk_parser_error(parser, "expected inline assembly body");

    return NULL;
}

s_tree_node* bonk_parse_grammatic_unary_operator(s_bonk_parser* parser) {
    s_bonk_lexeme* lexeme = bonk_parser_next_lexeme(parser);

    if(lexeme->type == BONK_LEXEME_KEYWORD) {

        if(lexeme->keyword_data.keyword_type == BONK_KEYWORD_BREK) {
            bonk_parser_eat_lexeme(parser);
            s_tree_node* brek = (s_tree_node*) tree_create_node_operator(BONK_OPERATOR_BREK);
            if(!brek) {
                compiler_out_of_memory(parser->compiler);
                return NULL;
            }

            brek->source_position = bonk_parser_position_clone(lexeme->position);
            return brek;
        }

        e_bonk_operator_type operator = 0;

        if(lexeme->keyword_data.keyword_type == BONK_KEYWORD_PRINT) {
            operator = BONK_OPERATOR_PRINT;
        } else if(lexeme->keyword_data.keyword_type == BONK_KEYWORD_BONK) {
            operator = BONK_OPERATOR_BONK;
        } else if(lexeme->keyword_data.keyword_type == BONK_KEYWORD_BAMS) {
            bonk_parser_eat_lexeme(parser);
            return bonk_parse_grammatic_bams(parser);
        } else {
            return NULL;
        }

        bonk_parser_eat_lexeme(parser);

        s_tree_node* expression = bonk_parse_grammatic_expression(parser);
        if(!expression) {
            if(parser->compiler->state) return NULL;
            bonk_parser_error(parser, "missing operand");
        }

        s_tree_node* print_call = (s_tree_node*) tree_create_node_operator(operator);
        if(!print_call) {
            compiler_out_of_memory(parser->compiler);
            tree_release_subtree(expression);
            return NULL;
        }
        print_call->source_position = bonk_parser_position_clone(lexeme->position);

        print_call->right = expression;

        return print_call;
    }

    return NULL;
}