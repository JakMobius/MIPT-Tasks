
/*
 * Reference grammatic
 * $REFERENCE = ('(' $EXPRESSION ')'|$VARNAME)('(' $EXPRESSION ')')?
 * $VARNAME = [a-zA-Z]+[a-zA-Z0-9]+
 */

#include "grammatic_reference.h"

s_tree_node* parse_grammatic_reference(s_parser* parser) {
    s_tree_node* variable = NULL;

    if(parser_next_char(parser) == '(') {
        parser_eat_char(parser);
        variable = parse_grammatic_expression(parser);
        if(parser_next_char(parser) != ')') {
            parser_error(parser, "expected ')'");
            tree_release_subtree(variable);
            return NULL;
        }
        if(variable == NULL) {
            parser_error(parser, "expected parameter");
            tree_release_subtree(variable);
            parser_eat_char(parser);
            return NULL;
        }
        parser_eat_char(parser);
    } else {
        variable = parse_grammatic_varname(parser);
        if(!variable) return NULL;
    }


    s_tree_node* parameter = NULL;
    if(variable == NULL) return NULL;
    if(parser_next_char(parser) == '(') {
        parser_eat_char(parser);
        parameter = parse_grammatic_expression(parser);
        if(parser_next_char(parser) != ')') {
            parser_error(parser, "expected ')'");
            tree_release_subtree(variable);
            return NULL;
        }
        if(parameter == NULL) {
            parser_error(parser, "expected parameter");
            tree_release_subtree(variable);
            parser_eat_char(parser);
            return NULL;
        }
        parser_eat_char(parser);
    }

    if(parameter) {
        s_tree_node* call = (s_tree_node*)tree_create_node_operator(TREE_OPERATOR_TYPE_CALL);

        if(!call) {
            compiler_out_of_memory(parser->compiler);
            return NULL;
        }

        call->left = variable;
        call->right = parameter;
        return call;
    }

    return variable;
}

s_tree_node* parse_grammatic_varname(s_parser* parser) {
    unsigned long old_position = parser->position.index;
    const char* name_start = parser->text + parser->position.index;

    while(true) {
        char c = parser_next_char(parser);
        if(isdigit(c)) {
            if(parser->position.index == old_position) {
                break;
            }
        } else if(!isalpha(c)) break;

        parser_eat_char(parser);
    }

    if(old_position == parser->position.index) return NULL;

    s_tree_node_variable* variable = tree_create_node_variable();
    if(!variable) {
        compiler_out_of_memory(parser->compiler);
        return NULL;
    }
    variable->variable_name = name_start;
    variable->name_length = parser->position.index - old_position;

    return (s_tree_node*) variable;
}