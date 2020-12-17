
/*
 * Reference grammatic
 * $REFERENCE = "@" ($VARNAME | "(" $EXPRESSION ")") ("[" $ARGUMENT ("," $ARGUMENT)* "]") | $VARNAME
 * $ARGUMENT = $VARNAME "=" $EXPRESSION
 * $VARNAME = [a-zA-Z_][a-zA-Z0-9_]+
 */

#include "bonk_grammatic_reference.h"

s_tree_node* bonk_parse_grammatic_reference(s_bonk_parser* parser) {
    s_tree_node* variable = NULL;

    s_bonk_lexeme* next = bonk_parser_next_lexeme(parser);

    bool is_call = false;

    if(next->type == BONK_LEXEME_CALL) {
        is_call = true;

        bonk_parser_eat_lexeme(parser);
        next = bonk_parser_next_lexeme(parser);
    }

    if(next->type == BONK_LEXEME_BRACE && next->brace_data.brace_type == BONK_BRACE_L_RB) {
        bonk_parser_eat_lexeme(parser);
        variable = bonk_parse_grammatic_expression(parser);

        next = bonk_parser_next_lexeme(parser);

        if(next->type != BONK_LEXEME_BRACE || next->brace_data.brace_type != BONK_BRACE_R_RB) {
            if(!parser->compiler->state) {
                bonk_parser_error(parser, "expected ')'");
            }

            tree_release_subtree(variable);
            return NULL;
        }
        if(variable == NULL) {
            if(!parser->compiler->state) {
                bonk_parser_error(parser, "empty parenthesis in expression context");
            }

            tree_release_subtree(variable);
            bonk_parser_eat_lexeme(parser);
            return NULL;
        }
        bonk_parser_eat_lexeme(parser);
    } else if(next->type == BONK_LEXEME_IDENTIFIER) {
        variable = (s_tree_node*) next->identifier_data.identifier;
        bonk_parser_eat_lexeme(parser);
    }

    if(variable == NULL) {
        if(is_call) {
            bonk_parser_error(parser, "expected block name to call");
        }
        return NULL;
    }

    next = bonk_parser_next_lexeme(parser);

    if(is_call) {
        s_tree_node* call = (s_tree_node*) tree_create_node_operator(BONK_OPERATOR_CALL);

        s_tree_node* arguments = (s_tree_node*) bonk_parse_grammatic_arguments(parser);

        if(!call) {
            compiler_out_of_memory(parser->compiler);
            return NULL;
        }

        call->left = variable;
        call->right = arguments;
        return call;
    }

    return variable;
}

s_tree_node* bonk_parse_grammatic_arguments(s_bonk_parser* parser) {

    s_bonk_lexeme* next = bonk_parser_next_lexeme(parser);
    if(next->type != BONK_LEXEME_BRACE || next->brace_data.brace_type != BONK_BRACE_L_SB) {
        return NULL;
    }

    bonk_parser_eat_lexeme(parser);

    s_tree_node* argument_list = tree_create_typed_node(TREE_NODE_TYPE_LIST);
    if(!argument_list) {
        compiler_out_of_memory(parser->compiler);
        return NULL;
    }

    next = bonk_parser_next_lexeme(parser);

    s_tree_node* current_position = argument_list;

    while(true) {

        s_tree_node* argument = tree_create_typed_node(TREE_NODE_TYPE_ARGUMENT);
        current_position->left = argument;
        current_position->right = tree_create_typed_node(TREE_NODE_TYPE_LIST);
        current_position = current_position->right;

        if(!current_position) {
            tree_release_subtree(argument_list);
            compiler_out_of_memory(parser->compiler);
            return NULL;
        }

        if(next->type != BONK_LEXEME_IDENTIFIER) {
            bonk_parser_error(parser, "expected parameter name");
            tree_release_subtree(argument_list);
            return NULL;
        }

        argument->left = (s_tree_node*) next->identifier_data.identifier;

        bonk_parser_eat_lexeme(parser);
        next = bonk_parser_next_lexeme(parser);

        if(next->type != BONK_LEXEME_OPERATOR || next->operator_data.operator_type != BONK_OPERATOR_ASSIGNMENT) {
            bonk_parser_error(parser, "expected parameter value");
            tree_release_subtree(argument_list);
            return NULL;
        }



        bonk_parser_eat_lexeme(parser);

        s_tree_node* parameter_value = bonk_parse_grammatic_expression(parser);
        if(!parameter_value) {
            if(!parser->compiler->state) {
                bonk_parser_error(parser, "expected parameter value");
            }
            return NULL;
        }

        argument->right = parameter_value;

        next = bonk_parser_next_lexeme(parser);
        if(next->type != BONK_LEXEME_COMMA) {
            break;
        }

        bonk_parser_eat_lexeme(parser);
        next = bonk_parser_next_lexeme(parser);
    }

    next = bonk_parser_next_lexeme(parser);
    if(next->type != BONK_LEXEME_BRACE || next->brace_data.brace_type != BONK_BRACE_R_SB) {
        tree_release_subtree(argument_list);
        bonk_parser_error(parser, "expected ']'");
        return NULL;
    }

    bonk_parser_eat_lexeme(parser);

    return argument_list;
}