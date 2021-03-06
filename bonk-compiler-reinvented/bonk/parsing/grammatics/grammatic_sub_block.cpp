
/*
 * Sub block grammatic
 * $SUB_BLOCK := $CONSIDER_EXPRESSION | $CYCLE_EXPRESSION | "{" $BLOCK "}"
 * $CONSIDER_EXPRESSION := "consider" $EXPR ("{" $BLOCK "}" $OTHERWISE_EXPRESSION) | $OTHERWISE_EXPRESSION
 * $OTHERWISE_EXPRESSION := "otherwise" ($CONSIDER_EXPRESSION | "{" $BLOCK "}")
 * $CYCLE_EXPRESSION := "cycle" "{" $BLOCK "}"
 */

#include "grammatic_sub_block.hpp"

namespace bonk {

tree_node* parse_grammatic_sub_block(parser* parser) {
    tree_node* expression = parse_grammatic_cycle(parser);
    if(expression) return expression;
    if(parser->linked_compiler->state) return nullptr;

    expression = parse_grammatic_check(parser);
    if(expression) return expression;
    if(parser->linked_compiler->state) return nullptr;

    return nullptr;
}

tree_node_check* parse_grammatic_check(parser* parser) {
    lexeme* next = parser->next_lexeme();

    if(next->type != BONK_LEXEME_KEYWORD || next->keyword_data.keyword_type != BONK_KEYWORD_CHECK) {
        return nullptr;
    }

    parser_position* check_position = next->position;
    parser->eat_lexeme();
    next = parser->next_lexeme();

    tree_node* condition = parse_grammatic_expression(parser);
    if(!condition) {
        if(!parser->linked_compiler->state) {
            parser->error("expected expression to check");
        }
        return nullptr;
    }

    auto* block = parse_grammatic_nested_block(parser);
    if(!block) {
        condition->release();
        if(!parser->linked_compiler->state) {
            parser->error("expected check body");
        }
        return nullptr;
    }

    auto* check = tree_node_check::create();
    if(!check) {
        block->release();
        parser->linked_compiler->out_of_memory();
        return nullptr;
    }

    tree_node_list<tree_node*>* else_block = nullptr;

    next = parser->next_lexeme();

    if(next->type == BONK_LEXEME_KEYWORD && next->keyword_data.keyword_type == BONK_KEYWORD_OR) {
        parser->eat_lexeme();
        else_block = parse_grammatic_nested_block(parser);
        if(!else_block) {
            condition->release();
            block->release();

            if(!parser->linked_compiler->state) {
                parser->error("expected or body");
            }
            return nullptr;
        }
    }

    check->source_position = check_position->clone();
    check->condition = condition;
    check->check_body = block;
    check->or_body = else_block;

    return check;
}

tree_node_cycle* parse_grammatic_cycle(parser* parser) {
    lexeme* next = parser->next_lexeme();

    if(next->type != BONK_LEXEME_KEYWORD || next->keyword_data.keyword_type != BONK_KEYWORD_CYCLE) {
        return nullptr;
    }

    parser_position* cycle_position = next->position;
    parser->eat_lexeme();

    auto* block = parse_grammatic_nested_block(parser);
    if(!block) {
        if(!parser->linked_compiler->state) parser->error("expected cycle body");
        return nullptr;
    }

    auto* cycle = tree_node_cycle::create();
    if(!cycle) {
        block->release();
        parser->linked_compiler->out_of_memory();
    }
    cycle->source_position = cycle_position->clone();
    cycle->body = block;
    return cycle;
}

tree_node_list<tree_node*>* parse_grammatic_nested_block(parser* parser) {
    lexeme* next = parser->next_lexeme();

    if(next->type != BONK_LEXEME_BRACE || next->brace_data.brace_type != BONK_BRACE_L_CB) {
        parser->error("expected '{'");
        return nullptr;
    }

    parser->eat_lexeme();

    tree_node_list<tree_node*>* block = parse_grammatic_block(parser);
    if(!block) {
        if(parser->linked_compiler->state) return nullptr;
        parser->error("expected block");
        return nullptr;
    }
    block->source_position = next->position->clone();

    next = parser->next_lexeme();

    if(next->type != BONK_LEXEME_BRACE || next->brace_data.brace_type != BONK_BRACE_R_CB) {
        parser->error("expected '}'");
        block->release();
        return nullptr;
    }

    parser->eat_lexeme();

    return block;
}

}