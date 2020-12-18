
#include "simplifier.h"
#include "tree.h"
#include <math.h>
#include <assert.h>
#include <stdbool.h>

const int TREE_NODE_OPERATOR_PRIORITY[] = {
    0, // TREE_OPERATOR_TYPE_PLUS
    1, // TREE_OPERATOR_TYPE_MINUS
    3, // TREE_OPERATOR_TYPE_MULTIPLY
    2, // TREE_OPERATOR_TYPE_DIVIDE
    3, // TREE_OPERATOR_TYPE_POW
    1, // TREE_OPERATOR_TYPE_DIFFERENTIAL
    4, // TREE_OPERATOR_TYPE_CALL
};

const char* TREE_NODE_OPERATOR_NAMES[] = {
    "+",    // TREE_OPERATOR_TYPE_PLUS
    "-",    // TREE_OPERATOR_TYPE_MINUS
    "*",    // TREE_OPERATOR_TYPE_MULTIPLY
    "/",    // TREE_OPERATOR_TYPE_DIVIDE
    "^",    // TREE_OPERATOR_TYPE_POW
    "d",    // TREE_OPERATOR_TYPE_DIFFERENTIAL
    "",     // TREE_OPERATOR_TYPE_CALL
    NULL
};

const char* TREE_CONSTANT_NAMES[] = {
    "x",
    "y",
    "z",
    "a",
    "b",
    "c",
    "e",
    "pi",
    NULL
};

const char* TREE_FUNCTION_NAMES[] = {
        "sin",
        "cos",
        "ln",
        "sqrt",
        NULL
};

const double TREE_CONSTANT_PI = M_PI;
const double TREE_CONSTANT_E = M_E;

const double* TREE_CONSTANT_VALUES[] = {
    NULL, // x
    NULL, // y
    NULL, // z
    NULL, // a
    NULL, // b
    NULL, // c
    &TREE_CONSTANT_E, // e
    &TREE_CONSTANT_PI // pi
};

const int TREE_CONSTANT_AMOUNT = 8;

const e_tree_node_operator_flag TREE_NODE_OPERATOR_FLAGS[] = {
// TREE_OPERATOR_TYPE_PLUS:
    TREE_OPERATOR_FLAG_LEFT_HAND |
    TREE_OPERATOR_FLAG_RIGHT_HAND |
    TREE_OPERATOR_FLAG_ASSOCIATIVE,
// TREE_OPERATOR_TYPE_MINUS
    TREE_OPERATOR_FLAG_LEFT_HAND_OPTIONAL |
    TREE_OPERATOR_FLAG_RIGHT_HAND,
// TREE_OPERATOR_TYPE_MULTIPLY
    TREE_OPERATOR_FLAG_LEFT_HAND |
    TREE_OPERATOR_FLAG_RIGHT_HAND |
    TREE_OPERATOR_FLAG_ASSOCIATIVE,
// TREE_OPERATOR_TYPE_DIVIDE
    TREE_OPERATOR_FLAG_LEFT_HAND |
    TREE_OPERATOR_FLAG_RIGHT_HAND |
    TREE_OPERATOR_FLAG_PACIFIC_GROUP,
// TREE_OPERATOR_TYPE_POW
    TREE_OPERATOR_FLAG_LEFT_HAND |
    TREE_OPERATOR_FLAG_RIGHT_HAND |
    TREE_OPERATOR_FLAG_STRONG_GROUP,
// TREE_OPERATOR_TYPE_DIFFERENTIAL
    TREE_OPERATOR_FLAG_RIGHT_HAND_OPTIONAL,
// TREE_OPERATOR_TYPE_CALL
    TREE_OPERATOR_FLAG_LEFT_HAND |
    TREE_OPERATOR_FLAG_RIGHT_HAND
};

e_tree_error wrap_error(e_tree_error error) {
    return error;
};

s_tree* tree_new() {

    s_tree* tree = calloc(1, sizeof(s_tree));

    if(!tree) return NULL;

    tree_construct(tree);

    return tree;
}

int tree_node_amount = 0;

e_tree_error tree_construct(s_tree* tree) {

    tree->root = NULL;
    return TREE_OK;
}

void tree_destruct(s_tree* tree) {
    tree_release_subtree(tree->root);
}

void tree_release(s_tree *tree) {
    if(!tree) return;
    tree_destruct(tree);
    free(tree);
}

s_tree_node_number* tree_create_number_from_double(double value) {
    s_tree_node_number* number = calloc(1, sizeof(s_tree_node_number));
    if(number == NULL) return NULL;
    tree_construct_node_number(number);
    number->integer_value = (long long)value;
    number->float_value = value;
    return number;
}

s_tree_node_number* tree_create_number_from_integer(long long value) {
    s_tree_node_number* number = calloc(1, sizeof(s_tree_node_number));
    if(number == NULL) return NULL;
    tree_construct_node_number(number);
    number->integer_value = value;
    number->float_value = (double)value;
    return number;
}

void tree_construct_node(s_tree_node* node) {
    node->type = -1;
    node->left = NULL;
    node->right = NULL;

    tree_node_amount++;
}

s_tree_node* tree_create_node() {
    s_tree_node* node = calloc(1, sizeof(s_tree_node));
    if(node == NULL) return NULL;
    tree_construct_node(node);
    return node;
}

s_tree_node_function* tree_create_node_function(e_tree_function function_type) {
    s_tree_node_function* function = calloc(1, sizeof(s_tree_node_function));
    if(function == NULL) return NULL;
    tree_construct_node_function(function, function_type);
    return function;
}

void tree_construct_node_function(s_tree_node_function* function, e_tree_function function_type) {
    tree_construct_node((s_tree_node*) function);
    function->tree_node.type = TREE_NODE_TYPE_FUNCTION;
    function->function = function_type;
}

void tree_construct_node_operator(s_tree_node_operator* operator) {
    tree_construct_node((s_tree_node*) operator);
    operator->operator_type = -1;
}

s_tree_node_operator* tree_create_node_operator(e_tree_node_operator_type type) {
    s_tree_node_operator* node = calloc(1, sizeof(s_tree_node_operator));
    if(node == NULL) return NULL;
    tree_construct_node_operator(node);
    node->tree_node.type = TREE_NODE_TYPE_OPERATOR;
    node->operator_type = type;
    return node;
}

void tree_construct_node_number(s_tree_node_number* number) {
    tree_construct_node((s_tree_node*) number);
    number->tree_node.type = TREE_NODE_TYPE_NUMBER;
    number->float_value = 0;
    number->integer_value = 0;
}

s_tree_node_number* tree_create_node_number() {
    s_tree_node_number* node = calloc(1, sizeof(s_tree_node_number));
    if(node == NULL) return NULL;
    tree_construct_node_number(node);
    return node;
}

void tree_construct_node_variable(s_tree_node_variable* variable) {
    tree_construct_node((s_tree_node*) variable);
    variable->tree_node.type = TREE_NODE_TYPE_VARIABLE;
    variable->variable_name = NULL;
    variable->resolved = false;
    variable->variable_index = 0;
}

s_tree_node_variable* tree_create_node_variable() {
    s_tree_node_variable* node = calloc(1, sizeof(s_tree_node_variable));
    if(node == NULL) return NULL;
    tree_construct_node_variable(node);
    return node;
}

e_tree_error tree_release_node(s_tree_node* node) {
    if(!node) return TREE_ERROR_NULL;


    free(node);

    return TREE_OK;
}

e_tree_error tree_release_subtree(s_tree_node* node) {
    if(!node) return TREE_ERROR_NULL;

    tree_node_amount--;

    tree_release_subtree(node->left);
    tree_release_subtree(node->right);
    tree_release_node(node);

    return TREE_OK;
}

void tree_node_sift_left(s_tree_node** node) {
    s_tree_node* release = *node;
    *node = (*node)->left;
    release->left = NULL;
    tree_release_subtree(release);
}

void tree_node_sift_right(s_tree_node** node) {
    s_tree_node* release = *node;
    *node = (*node)->right;
    release->right = NULL;
    tree_release_subtree(release);
}

s_tree_node* tree_left_leap(s_tree_node* node) {
    if(!node) return NULL;
    while(node->left) node = node->left;
    return node;
}

s_tree_node* tree_right_leap(s_tree_node* node) {
    if(!node) return NULL;
    while(node->left) node = node->left;
    return node;
}

bool tree_node_is_exact_number(s_tree_node* node, double value) {
    if(node->type == TREE_NODE_TYPE_NUMBER) {
        return ((s_tree_node_number*)node)->float_value == value;
    }
    return false;
}

s_tree_node* tree_node_copy(s_tree_node* node) {
    unsigned long size = tree_node_size(node);
    s_tree_node* new_node = calloc(1, size);
    if(new_node == NULL) return NULL;
    memcpy(new_node, node, size);

    return new_node;
}

unsigned long tree_node_size(s_tree_node* node) {
    switch(node->type) {
        case TREE_NODE_TYPE_NUMBER:
            return sizeof(s_tree_node_number);
        case TREE_NODE_TYPE_VARIABLE:
            return sizeof(s_tree_node_variable);
        case TREE_NODE_TYPE_OPERATOR:
            return sizeof(s_tree_node_operator);
        case TREE_NODE_TYPE_FUNCTION:
            return sizeof(s_tree_node_function);
    }
}

s_tree_node* tree_node_clone_deep(s_tree_node* node) {
    if(node == NULL) return NULL;

    unsigned long type_size = tree_node_size(node);
    s_tree_node* clone = calloc(1, type_size);
    if(!clone) return NULL;
    tree_node_amount++;

    clone->type = node->type;
    clone->left = tree_node_clone_deep(node->left);
    clone->right = tree_node_clone_deep(node->right);

    unsigned long bytes_to_copy = type_size - sizeof(s_tree_node);
    memcpy((char*)clone + sizeof(s_tree_node), (char*)node + sizeof(s_tree_node), bytes_to_copy);

    if((!clone->left && node->left) || (!clone->right && node->right)) {
        tree_release_node(clone);
        return NULL;
    }

    return clone;
}

bool tree_node_is_operator(s_tree_node* node, e_tree_node_operator_type operator) {
    if(node->type != TREE_NODE_TYPE_OPERATOR) return false;
    return ((s_tree_node_operator*)node)->operator_type == operator;
}

void subtree_validate(s_tree_node* node, s_tree_node* parent) {
    if(!node) return;
    assert(node->parent_reference == 0);
    node->parent_reference = parent;
    subtree_validate(node->left, node);
    subtree_validate(node->right, node);
}

void subtree_reference_count_reset(s_tree_node* node) {
    if(!node) return;
    node->parent_reference = NULL;
    subtree_reference_count_reset(node->left);
    subtree_reference_count_reset(node->right);
}

void tree_validate(s_tree* tree) {
    subtree_validate(tree->root, NULL);
    subtree_reference_count_reset(tree->root);

}

e_tree_error tree_evaluate(s_tree* tree, const double** variables, double* value) {
    if(!tree) return TREE_ERROR_NULL;

    s_tree_evaluation_context context = {0};
    context.variables = variables;

    return tree_evaluate_node(tree->root, &context, value);
}

e_tree_error tree_evaluate_node(s_tree_node* node, s_tree_evaluation_context* context, double* result) {
    if(!node) return TREE_ERROR_NULL;

    double left_value = 0;
    double right_value = 0;
    e_tree_error error = TREE_OK;

    if(node->type == TREE_NODE_TYPE_VARIABLE) {
        s_tree_node_variable* variable = (s_tree_node_variable*) node;
        if(TREE_CONSTANT_VALUES[variable->variable_index]) {
            *result = *TREE_CONSTANT_VALUES[variable->variable_index];
            return TREE_OK;
        }
        if(context->variables[variable->variable_index] == NULL) return TREE_ERROR_CANNOT_EVALUATE;
        *result = *context->variables[variable->variable_index];
        return TREE_OK;
    }

    if(node->type == TREE_NODE_TYPE_NUMBER) {
        s_tree_node_number* number = (s_tree_node_number*) node;
        *result = (double) number->float_value;
        return TREE_OK;
    }

    if(node->type == TREE_NODE_TYPE_OPERATOR) {

        s_tree_node_operator* operator = (s_tree_node_operator*) node;

        if(node->right) {
            if((error = tree_evaluate_node(node->right, context, &right_value))) return error;
        }

        switch(operator->operator_type) {

            case TREE_OPERATOR_TYPE_PLUS:
                if((error = tree_evaluate_node(node->left, context, &left_value))) return error;
                *result = left_value + right_value;
                break;
            case TREE_OPERATOR_TYPE_MINUS:
                if(node->left && (error = tree_evaluate_node(node->left, context, &left_value))) return error;
                *result = left_value - right_value;
                break;
            case TREE_OPERATOR_TYPE_MULTIPLY:
                if((error = tree_evaluate_node(node->left, context, &left_value))) return error;
                *result = left_value * right_value;
                break;
            case TREE_OPERATOR_TYPE_DIVIDE:
                if(right_value == 0) return TREE_ERROR_CANNOT_EVALUATE;
                if((error = tree_evaluate_node(node->left, context, &left_value))) return error;
                *result = left_value / right_value;
                break;
            case TREE_OPERATOR_TYPE_CALL:
                if(node->left->type != TREE_NODE_TYPE_FUNCTION) return TREE_ERROR_CANNOT_EVALUATE;
                s_tree_node_function* function = (s_tree_node_function*)node->left;

                switch(function->function) {
                    case TREE_FUNCTION_SIN:
                        *result = sin(right_value);
                        return TREE_OK;
                    case TREE_FUNCTION_COS:
                        *result = cos(right_value);
                        return TREE_OK;
                    case TREE_FUNCTION_LN:
                        if(right_value <= 0) return TREE_ERROR_CANNOT_EVALUATE;
                        *result = log2(right_value);
                        return TREE_OK;
                    case TREE_FUNCTION_SQRT:
                        if(right_value < 0) return TREE_ERROR_CANNOT_EVALUATE;
                        *result = sqrt(right_value);
                        return TREE_OK;
                }

            case TREE_OPERATOR_TYPE_POW:
                if((error = tree_evaluate_node(node->left, context, &left_value))) return error;
                if(left_value < 0 && right_value != round(right_value)) {
                    if(tree_node_is_operator(node->right, TREE_OPERATOR_TYPE_DIVIDE)) {
                        double numerator = 0;
                        double denominator = 0;

                        if((error = tree_evaluate_node(node->right->left, context, &numerator))) return error;
                        if((error = tree_evaluate_node(node->right->right, context, &denominator))) return error;

                        if(numerator == (int)numerator && denominator == (int)denominator && ((int)denominator) % 2 == 1) {
                            *result = pow(-left_value, numerator/denominator);
                            if(((int)numerator) % 2 == 1) {
                                *result = -*result;
                            }
                            return TREE_OK;
                        }
                    }
                    return TREE_ERROR_CANNOT_EVALUATE;
                }
                *result = pow(left_value, right_value);
                return TREE_OK;
            case TREE_OPERATOR_TYPE_DIFFERENTIAL:
                return TREE_ERROR_CANNOT_EVALUATE;
        }
    }

    return TREE_OK;
}