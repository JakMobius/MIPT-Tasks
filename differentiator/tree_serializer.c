
#include "tree_serializer.h"

e_tree_error tree_parser_serialize_subtree(s_tree_node* target, FILE* file) {
    if(target == NULL) return TREE_ERROR_NULL;
    if(file == NULL) return TREE_ERROR_NULL;

    if(target->type == TREE_NODE_TYPE_NUMBER) {
        s_tree_node_number* number = (s_tree_node_number*) target;
        fprintf(file, "%lg", (double)number->float_value);
    } else if(target->type == TREE_NODE_TYPE_OPERATOR) {
        s_tree_node_operator* operator = (s_tree_node_operator*) target;
        fprintf(file, "(");
        if(target->left) tree_parser_serialize_subtree(target->left, file);
        if(operator->operator_type == -1) {
            fprintf(file, " <invalid operator> ");
        } else if(operator->operator_type != TREE_OPERATOR_TYPE_CALL){
            fprintf(file, "%s", TREE_NODE_OPERATOR_NAMES[operator->operator_type]);
        } else fprintf(file, "(");
        if(target->right) tree_parser_serialize_subtree(target->right, file);
        if(operator->operator_type == TREE_OPERATOR_TYPE_CALL) fprintf(file, ")");
        fprintf(file, ")");
    } else if(target->type == TREE_NODE_TYPE_VARIABLE) {
        s_tree_node_variable* variable = (s_tree_node_variable*) target;
        fprintf(file, "%s", TREE_CONSTANT_NAMES[variable->variable_index]);
    } else if(target->type == TREE_NODE_TYPE_FUNCTION) {
        s_tree_node_function* function = (s_tree_node_function*) target;

        fprintf(file, "%s", TREE_FUNCTION_NAMES[function->function]);
    } else {
        printf(" <invalid node> ");
    }

    return TREE_OK;
}

e_tree_error tree_parser_serialize(s_tree* target, FILE* file) {
    if(target == NULL) return TREE_ERROR_NULL;
    if(file == NULL) return TREE_ERROR_NULL;

    return tree_parser_serialize_subtree(target->root, file);
}

e_tree_error latex_dump_subexpression(s_tree_node* target, FILE* file, s_tree_node_operator* parent) {
    if(target == NULL) return TREE_ERROR_NULL;
    if(file == NULL) return TREE_ERROR_NULL;

    if(target->type == TREE_NODE_TYPE_NUMBER) {
        s_tree_node_number* number = (s_tree_node_number*) target;
        fprintf(file, "{%lg}", (double)number->float_value);
    } else if(target->type == TREE_NODE_TYPE_OPERATOR) {

        s_tree_node_operator* operator = (s_tree_node_operator*) target;

        int priority = TREE_NODE_OPERATOR_PRIORITY[operator->operator_type];
        bool braces = false;

        if(parent && TREE_NODE_OPERATOR_PRIORITY[parent->operator_type] >= priority && parent->operator_type != operator->operator_type) {
            if(!(TREE_NODE_OPERATOR_FLAGS[parent->operator_type] & TREE_OPERATOR_FLAG_PACIFIC_GROUP)) {
                braces = true;
            }
        }

        if(TREE_NODE_OPERATOR_FLAGS[operator->operator_type] & TREE_OPERATOR_FLAG_STRONG_GROUP) braces = false;

        fprintf(file, "{");
        if(braces) fprintf(file, "\\left(");

        switch(operator->operator_type) {
            case TREE_OPERATOR_TYPE_PLUS:
            case TREE_OPERATOR_TYPE_MINUS:
                if(target->left) latex_dump_subexpression(target->left, file, operator);

                if(tree_node_is_operator(target->right, TREE_OPERATOR_TYPE_MINUS) && !target->right->left) {
                    if(operator->operator_type == TREE_OPERATOR_TYPE_PLUS) {
                        fprintf(file, "%s", TREE_NODE_OPERATOR_NAMES[TREE_OPERATOR_TYPE_MINUS]);
                    } else {
                        fprintf(file, "%s", TREE_NODE_OPERATOR_NAMES[TREE_OPERATOR_TYPE_PLUS]);
                    }
                    latex_dump_subexpression(target->right->right, file, operator);
                } else {
                    fprintf(file, "%s", TREE_NODE_OPERATOR_NAMES[operator->operator_type]);
                    latex_dump_subexpression(target->right, file, operator);
                }
                break;
            case TREE_OPERATOR_TYPE_CALL:
                latex_dump_subexpression(target->left, file, operator);
                fprintf(file, "{");
                latex_dump_subexpression(target->right, file, operator);
                fprintf(file, "}");
                break;
            case TREE_OPERATOR_TYPE_POW:
                if(target->left) latex_dump_subexpression(target->left, file, operator);
                fprintf(file, "^");
                if(target->right) latex_dump_subexpression(target->right, file, operator);
                break;

            case TREE_OPERATOR_TYPE_MULTIPLY:
                latex_dump_subexpression(target->left, file, operator);

                if(tree_left_leap(target->right)->type == TREE_NODE_TYPE_NUMBER) {
                    fprintf(file, "{\\cdot}");
                }

                latex_dump_subexpression(target->right, file, operator);
                break;
            case TREE_OPERATOR_TYPE_DIFFERENTIAL:
                fprintf(file, "%s", TREE_NODE_OPERATOR_NAMES[operator->operator_type]);
                if(target->right) {
                    fprintf(file, "{");
                    latex_dump_subexpression(target->right, file, operator);
                    fprintf(file, "}");
                }
                break;
            case TREE_OPERATOR_TYPE_DIVIDE:
                fprintf(file, "\\frac{");
                latex_dump_subexpression(target->left, file, operator);
                fprintf(file, "}{");
                latex_dump_subexpression(target->right, file, operator);
                fprintf(file, "}");
                break;
        }

        if(braces) fprintf(file, "\\right)");
        fprintf(file, "}");
    } else if(target->type == TREE_NODE_TYPE_VARIABLE) {
        s_tree_node_variable* variable = (s_tree_node_variable*) target;
        fprintf(file, "{%s}", TREE_CONSTANT_NAMES[variable->variable_index]);
    } else if(target->type == TREE_NODE_TYPE_FUNCTION) {
        s_tree_node_function* function = (s_tree_node_function*) target;

        fprintf(file, "\\%s", TREE_FUNCTION_NAMES[function->function]);
    }

    return TREE_OK;
}

void latex_document_begin(FILE* file) {
    fprintf(file, "\\documentclass{article}\n"
                  "\\usepackage[T2A]{fontenc}\n"
                  "\\usepackage[utf8]{inputenc}\n"
                  "\\usepackage[russian]{babel}\n"
                  "\\usepackage{amsmath}\n"
                  "\\usepackage{graphicx}\n"
                  "\\usepackage[legalpaper, landscape, margin=1in]{geometry}\n"
                  "\\begin{document}\n"
                  "\\allowdisplaybreaks\n");
}

void latex_add_image(const char* name, FILE* file) {
    fprintf(file, "\\centering"
                  "\\includegraphics[scale=0.5]{%s}\n", name);
}

void latex_expression_begin(FILE* file) {
    fprintf(file, "\\begin{gather*}\n");
}

void latex_document_add_expression(s_tree* expression, FILE* file) {
    latex_dump_subexpression(expression->root, file, false);
}

void latex_expression_break(FILE* file) {
    fprintf(file, "\\\\\n");
}

void latex_expression_end(FILE* file) {
    fprintf(file, "\\end{gather*}\n");
}

void latex_document_end(FILE* file) {
    fprintf(file, "\\end{document}");
}

