
#include "tree.h"
#include <assert.h>
#include <stdbool.h>

e_tree_error wrap_error(e_tree_error error) {
    assert(false);
};

s_tree* tree_new() {

    s_tree* tree = calloc(1, sizeof(s_tree));

    if(!tree) return NULL;

    tree_construct(tree);

    return tree;
}

e_tree_error tree_construct(s_tree* tree) {

    tree->root = NULL;

    return TREE_OK;
}

void tree_destruct(s_tree* tree) {
    tree_node_release(tree->root);
}

void tree_release(s_tree *tree) {
    if(!tree) return;
    tree_destruct(tree);
    free(tree);
}

void tree_node_release(s_tree_node *node) {
    if(!node) return;
    tree_node_release(node->left);
    tree_node_release(node->right);
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

s_tree_node_operator* tree_create_operator(e_bonk_operator_type type) {
    s_tree_node_operator* operator = calloc(1, sizeof(s_tree_node_operator));
    if(operator == NULL) return NULL;
    tree_construct_node_operator(operator);
    operator->operator_type = type;
    return operator;
}

void tree_construct_node(s_tree_node* node) {
    node->type = -1;
    node->left = NULL;
    node->right = NULL;
}

s_tree_node* tree_create_node() {
    s_tree_node* node = calloc(1, sizeof(s_tree_node));
    if(node == NULL) return NULL;
    tree_construct_node(node);
    return node;
}

s_tree_node* tree_create_typed_node(e_tree_node_type type) {
    s_tree_node* node = tree_create_node();
    if(!node) return NULL;
    node->type = type;
    return node;
}

void tree_construct_node_variable_definition(s_tree_node_variable_definition* variable_definition) {
    tree_construct_node((s_tree_node*) variable_definition);
    variable_definition->is_contextual = false;
}

s_tree_node_variable_definition* tree_create_node_variable_definition() {
    s_tree_node_variable_definition* variable_definition = calloc(1, sizeof(s_tree_node_variable_definition));
    if(variable_definition == NULL) return NULL;
    tree_construct_node_variable_definition(variable_definition);
    variable_definition->tree_node.type = TREE_NODE_TYPE_VAR_DEFINITION;
    return variable_definition;
}

void tree_construct_node_operator(s_tree_node_operator* operator) {
    tree_construct_node((s_tree_node*) operator);
    operator->operator_type = -1;
}

s_tree_node_operator* tree_create_node_operator(e_bonk_operator_type type) {
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

void tree_construct_node_identifier(s_tree_node_identifier* variable) {
    tree_construct_node((s_tree_node*) variable);
    variable->tree_node.type = TREE_NODE_TYPE_IDENTIFIER;
    variable->variable_name = NULL;
    variable->variable_index = 0;
}

s_tree_node_identifier* tree_create_node_identifier() {
    s_tree_node_identifier* node = calloc(1, sizeof(s_tree_node_identifier));
    if(node == NULL) return NULL;
    tree_construct_node_identifier(node);
    return node;
}

const char* tree_node_identifier_name(s_tree_node_identifier* identifier) {
    return strndup(identifier->variable_name, identifier->name_length);
}

bool tree_node_identifier_equals(s_tree_node_identifier* left, s_tree_node_identifier* right) {
    if(left->name_length != right->name_length) return false;
    return strncmp(left->variable_name, right->variable_name, left->name_length) == 0;
}

void bonk_print_identifier(FILE* file, s_tree_node_identifier* identifier) {
    fwrite(identifier->variable_name, 1, identifier->name_length, file);
}

e_tree_error tree_release_node(s_tree_node* node) {
    if(!node) return TREE_ERROR_NULL;

    free(node);

    return TREE_OK;
}

e_tree_error tree_release_subtree(s_tree_node* node) {
    if(!node) return TREE_ERROR_NULL;

    tree_release_subtree(node->left);
    tree_release_subtree(node->right);
    tree_release_node(node);

    return TREE_OK;
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
    subtree_reference_count_reset(tree->root);
    subtree_validate(tree->root, NULL);
}

s_tree_node* tree_node_clone(s_tree_node* node) {
    if(node == NULL) return NULL;

    s_tree_node* clone = tree_create_node();
    if(!clone) return NULL;

    clone->type = node->type;
    clone->left = tree_node_clone(node->left);
    clone->right = tree_node_clone(node->right);

    unsigned long bytes_to_copy = 0;
    switch(node->type) {
        case TREE_NODE_TYPE_NUMBER:
            bytes_to_copy = sizeof(s_tree_node_number);
            break;
        case TREE_NODE_TYPE_IDENTIFIER:
            bytes_to_copy = sizeof(s_tree_node_identifier);
            break;
        case TREE_NODE_TYPE_OPERATOR:
            bytes_to_copy = sizeof(s_tree_node_operator);
            break;
        case TREE_NODE_TYPE_LIST:
        case TREE_NODE_TYPE_BLOCK_DEFINITION:
        case TREE_NODE_TYPE_VAR_DEFINITION:
        case TREE_NODE_TYPE_ASSIGNMENT:
        case TREE_NODE_TYPE_ARGUMENT:
            bytes_to_copy = sizeof(s_tree_node);
            break;
    }
    bytes_to_copy -= sizeof(s_tree_node);
    memcpy((char*)clone + sizeof(s_tree_node), (char*)node + sizeof(s_tree_node), bytes_to_copy);

    if((!clone->left && node->left) || (!clone->right && node->right)) {
        tree_release_node(clone);
        return NULL;
    }

    return clone;
}

e_tree_error tree_serialize_subtree(s_tree_node* target, FILE* file) {
    if(target == NULL) return TREE_ERROR_NULL;
    if(file == NULL) return TREE_ERROR_NULL;

    switch(target->type) {

        case TREE_NODE_TYPE_NUMBER: {
                s_tree_node_number* number = (s_tree_node_number*) target;
                fprintf(file, "%Lg", number->float_value);
            }
            break;
        case TREE_NODE_TYPE_IDENTIFIER:
            {
                bonk_print_identifier(file, (s_tree_node_identifier*) target);
            }
            break;
        case TREE_NODE_TYPE_OPERATOR:
            {
                s_tree_node_operator* operator = (s_tree_node_operator*) target;
                fprintf(file, "(");

                if(target->left) tree_serialize_subtree(target->left, file);

                if(operator->operator_type == -1) {
                    fprintf(file, " <invalid operator> ");
                } else if(operator->operator_type != BONK_OPERATOR_CALL) {
                    fprintf(file, "%s", BONK_OPERATOR_NAMES[operator->operator_type]);
                }

                if(operator->operator_type == BONK_OPERATOR_CALL) fprintf(file, "(");

                if(target->right) tree_serialize_subtree(target->right, file);

                if(operator->operator_type == BONK_OPERATOR_CALL) fprintf(file, ")");

                fprintf(file, ")");
            }
            break;
        case TREE_NODE_TYPE_LIST:
            {
                if(target->left) {
                    tree_serialize_subtree(target->left, file);
                    fprintf(file, ";");
                }
                if(target->right) tree_serialize_subtree(target->right, file);
            }
            break;
        case TREE_NODE_TYPE_BLOCK_DEFINITION:
            {
                fprintf(file, "block ");
                tree_serialize_subtree(target->left, file);
                fprintf(file, "{");
                tree_serialize_subtree(target->right, file);
                fprintf(file, "}");
            }
            break;
        case TREE_NODE_TYPE_VAR_DEFINITION:
            {
                fprintf(file, "var ");
                tree_serialize_subtree(target->left, file);
            }
            break;
        case TREE_NODE_TYPE_ASSIGNMENT:
            {
                tree_serialize_subtree(target->left, file);
                fprintf(file, "=");
                tree_serialize_subtree(target->right, file);
            }
            break;
        default:
            fprintf(file, " <invalid node> ");
    }

    return TREE_OK;
}

e_tree_error tree_serialize(s_tree* target, FILE* file) {
    if(target == NULL) return TREE_ERROR_NULL;
    if(file == NULL) return TREE_ERROR_NULL;

    return tree_serialize_subtree(target->root, file);
}