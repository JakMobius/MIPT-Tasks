
/*
 * Sub block grammatic
 * $SUB_BLOCK := $CONSIDER_EXPRESSION | $CYCLE_EXPRESSION | "{" $BLOCK "}"
 * $CONSIDER_EXPRESSION := "consider" $EXPR ("{" $BLOCK "}" $OTHERWISE_EXPRESSION) | $OTHERWISE_EXPRESSION
 * $OTHERWISE_EXPRESSION := "otherwise" ($CONSIDER_EXPRESSION | "{" $BLOCK "}")
 * $CYCLE_EXPRESSION := "cycle" "{" $BLOCK "}"
 */

#include "bonk_grammatic_sub_block.h"

s_tree_node* bonk_parse_grammatic_sub_block(s_bonk_parser* parser) {
    s_tree_node* expression = bonk_parse_grammatic_cycle(parser);
    if(expression) return expression;
    if(parser->compiler->state) return NULL;

    expression = bonk_parse_grammatic_check(parser);
    if(expression) return expression;
    if(parser->compiler->state) return NULL;



    return NULL;
}

s_tree_node* bonk_parse_grammatic_check(s_bonk_parser* parser) {
    s_bonk_lexeme* next = bonk_parser_next_lexeme(parser);

    if(next->type != BONK_LEXEME_KEYWORD || next->keyword_data.keyword_type != BONK_KEYWORD_CHECK) {
        return NULL;
    }

    s_bonk_parser_position* check_position = next->position;
    bonk_parser_eat_lexeme(parser);
    next = bonk_parser_next_lexeme(parser);

    s_tree_node* condition = bonk_parse_grammatic_expression(parser);
    if(!condition) {
        if(!parser->compiler->state) {
            bonk_parser_error(parser, "expected expression to check");
        }
        return NULL;
    }

    s_tree_node* block = bonk_parse_grammatic_nested_block(parser);
    if(!block) {
        tree_release_subtree(condition);
        if(!parser->compiler->state) {
            bonk_parser_error(parser, "expected check body");
        }
        return NULL;
    }

    s_tree_node* check = (s_tree_node*) tree_create_node_operator(BONK_OPERATOR_CHECK);
    if(!check) {
        tree_release_subtree(block);
        compiler_out_of_memory(parser->compiler);
        return NULL;
    }

    s_tree_node* else_block = NULL;

    next = bonk_parser_next_lexeme(parser);

    if(next->type == BONK_LEXEME_KEYWORD && next->keyword_data.keyword_type == BONK_KEYWORD_OR) {
        bonk_parser_eat_lexeme(parser);
        else_block = (s_tree_node*) bonk_parse_grammatic_nested_block(parser);
        if(!else_block) {
            tree_release_subtree(condition);
            tree_release_subtree(block);

            if(!parser->compiler->state) {
                bonk_parser_error(parser, "expected or body");
            }
            return NULL;
        }
    }

    check->source_position = bonk_parser_position_clone(check_position);
    check->left = condition;
    check->right = tree_create_typed_node(TREE_NODE_TYPE_LIST);

    if(!check->right) {
        compiler_out_of_memory(parser->compiler);
        tree_release_subtree(condition);
        tree_release_subtree(block);
        tree_release_subtree(else_block);
        return NULL;
    }

    check->right->left = block;
    check->right->right = else_block;
    return check;
}

s_tree_node* bonk_parse_grammatic_cycle(s_bonk_parser* parser) {
    s_bonk_lexeme* next = bonk_parser_next_lexeme(parser);

    if(next->type != BONK_LEXEME_KEYWORD || next->keyword_data.keyword_type != BONK_KEYWORD_CYCLE) {
        return NULL;
    }

    s_bonk_parser_position* cycle_position = next->position;
    bonk_parser_eat_lexeme(parser);
    next = bonk_parser_next_lexeme(parser);

    s_tree_node* block = bonk_parse_grammatic_nested_block(parser);
    if(!block) {
        if(parser->compiler->state) return NULL;
    }

    s_tree_node* cycle = (s_tree_node*) tree_create_node_operator(BONK_OPERATOR_CYCLE);
    if(!cycle) {
        tree_release_subtree(block);
        compiler_out_of_memory(parser->compiler);
    }
    cycle->source_position = bonk_parser_position_clone(cycle_position);
    cycle->right = block;
    return cycle;
}

s_tree_node* bonk_parse_grammatic_nested_block(s_bonk_parser* parser) {
    s_bonk_lexeme* next = bonk_parser_next_lexeme(parser);

    if(next->type != BONK_LEXEME_BRACE || next->brace_data.brace_type != BONK_BRACE_L_CB) {
        bonk_parser_error(parser, "expected '{'");
        return NULL;
    }

    bonk_parser_eat_lexeme(parser);

    s_tree_node* block = bonk_parse_grammatic_block(parser);
    if(!block) {
        if(parser->compiler->state) return NULL;
    }
    block->source_position = bonk_parser_position_clone(next->position);

    next = bonk_parser_next_lexeme(parser);

    if(next->type != BONK_LEXEME_BRACE || next->brace_data.brace_type != BONK_BRACE_R_CB) {
        bonk_parser_error(parser, "expected '}'");
        tree_release_subtree(block);
        return NULL;
    }

    bonk_parser_eat_lexeme(parser);

    return block;
}