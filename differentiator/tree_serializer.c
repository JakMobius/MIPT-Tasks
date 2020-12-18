
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

int get_priority(s_tree_node_operator* node) {
    switch(node->operator_type) {
        case TREE_OPERATOR_TYPE_PLUS:         return 0;
        case TREE_OPERATOR_TYPE_MINUS:        return 1;
        case TREE_OPERATOR_TYPE_MULTIPLY:     return 3;
        case TREE_OPERATOR_TYPE_DIVIDE:       return 2;
        case TREE_OPERATOR_TYPE_POW:          return 3;
        case TREE_OPERATOR_TYPE_DIFFERENTIAL: return 1;
        case TREE_OPERATOR_TYPE_CALL:
            if(tree_is_differential(node->tree_node.left)) return 1;
            return 4;
    }
}

e_tree_error latex_dump_subexpression(s_tree_node* target, FILE* file, s_tree_node_operator* parent, s_tree_node* highlight) {
    if(target == NULL) return TREE_ERROR_NULL;
    if(file == NULL) return TREE_ERROR_NULL;

    if(highlight == target) {
        fprintf(file, "{\\color{red}{");
    }

    if(target->type == TREE_NODE_TYPE_NUMBER) {
        s_tree_node_number* number = (s_tree_node_number*) target;
        fprintf(file, "{%lg}", (double)number->float_value);
    } else if(target->type == TREE_NODE_TYPE_OPERATOR) {

        s_tree_node_operator* operator = (s_tree_node_operator*) target;

        int priority = get_priority(operator);
        bool braces = false;

        if(parent && (get_priority(parent) >= priority && parent->operator_type != operator->operator_type)) {
            if(!(TREE_NODE_OPERATOR_FLAGS[parent->operator_type] & TREE_OPERATOR_FLAG_PACIFIC_GROUP)) {
                braces = true;
            }
        }
        if(parent && parent->operator_type == TREE_OPERATOR_TYPE_POW && !braces) {
            braces = operator->operator_type == TREE_OPERATOR_TYPE_POW;
        }

        if(TREE_NODE_OPERATOR_FLAGS[operator->operator_type] & TREE_OPERATOR_FLAG_STRONG_GROUP) braces = false;

        fprintf(file, "{");
        if(braces) fprintf(file, "\\left(");

        switch(operator->operator_type) {
            case TREE_OPERATOR_TYPE_PLUS:
            case TREE_OPERATOR_TYPE_MINUS:
                if(target->left) latex_dump_subexpression(target->left, file, operator, highlight);

                if(tree_node_is_operator(target->right, TREE_OPERATOR_TYPE_MINUS) && !target->right->left) {
                    if(operator->operator_type == TREE_OPERATOR_TYPE_PLUS) {
                        fprintf(file, "%s", TREE_NODE_OPERATOR_NAMES[TREE_OPERATOR_TYPE_MINUS]);
                    } else {
                        fprintf(file, "%s", TREE_NODE_OPERATOR_NAMES[TREE_OPERATOR_TYPE_PLUS]);
                    }
                    latex_dump_subexpression(target->right->right, file, operator, highlight);
                } else {
                    fprintf(file, "%s", TREE_NODE_OPERATOR_NAMES[operator->operator_type]);
                    latex_dump_subexpression(target->right, file, operator, highlight);
                }
                break;
            case TREE_OPERATOR_TYPE_CALL:
                latex_dump_subexpression(target->left, file, operator, highlight);
                fprintf(file, "{");
                latex_dump_subexpression(target->right, file, operator, highlight);
                fprintf(file, "}");
                break;
            case TREE_OPERATOR_TYPE_POW:
                if(target->left) latex_dump_subexpression(target->left, file, operator, highlight);
                fprintf(file, "^");
                if(target->right) latex_dump_subexpression(target->right, file, operator, highlight);
                break;

            case TREE_OPERATOR_TYPE_MULTIPLY:
                latex_dump_subexpression(target->left, file, operator, highlight);

                if(tree_left_leap(target->right)->type == TREE_NODE_TYPE_NUMBER) {
                    fprintf(file, "{\\cdot}");
                }

                latex_dump_subexpression(target->right, file, operator, highlight);
                break;
            case TREE_OPERATOR_TYPE_DIFFERENTIAL:
                fprintf(file, "%s", TREE_NODE_OPERATOR_NAMES[operator->operator_type]);
                if(target->right) {
                    fprintf(file, "{");
                    latex_dump_subexpression(target->right, file, operator, highlight);
                    fprintf(file, "}");
                }
                break;
            case TREE_OPERATOR_TYPE_DIVIDE:
                fprintf(file, "\\frac{");
                latex_dump_subexpression(target->left, file, operator, highlight);
                fprintf(file, "}{");
                latex_dump_subexpression(target->right, file, operator, highlight);
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

    if(highlight == target) {
        fprintf(file, "}}");
    }

    return TREE_OK;
}

void latex_document_begin(FILE* file) {
    fprintf(file, "\\documentclass{article}\n"
                  "\\title{Современные методы анализа и применения современных методов анализов и применений в контексте "
                  "применения современных методов упрощения и приведения к упрощенному виду выражений, примененных к "
                  "анализу современных методов}\n"
                  "\\date{2077-95-07}\n"
                  "\\author{Albert Einstein}\n"
                  "\\usepackage[T2A]{fontenc}\n"
                  "\\usepackage[utf8]{inputenc}\n"
                  "\\usepackage[russian]{babel}\n"
                  "\\usepackage{amsmath}\n"
                  "\\usepackage{graphicx}\n"
                  "\\usepackage{hyperref}\n"
                  "\\usepackage{xcolor}\n"
                  "\\usepackage[legalpaper,portrait,margin=1in]{geometry}\n"
                  "\\begin{document}\n"
                  "\\maketitle\n"
                  "\\newpage\n"
                  "\\allowdisplaybreaks\n");
}

void latex_add_image(const char* name, FILE* file) {
    fprintf(file, "\\begin{center}\\includegraphics[scale=0.5]{%s}\n\\end{center}", name);
}

void latex_expression_begin(FILE* file) {
    fprintf(file, "\\begin{gather*}\n");
}

void latex_document_add_expression(s_tree* expression, FILE* file, s_tree_node* highlight) {
    latex_dump_subexpression(expression->root, file, false, highlight);
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

