
#include "compiler.h"

void compiler_out_of_memory(s_compiler* compiler) {
    compiler_fatal_error(compiler, "Compiler ran out of memory");
};

s_compiler* compiler_create(s_compiler_config* config) {
    s_compiler* compiler = calloc(1, sizeof(s_parser));
    compiler_construct(compiler, config);
    return compiler;
}

void compiler_construct(s_compiler* compiler, s_compiler_config* config) {
    compiler->config = config;
    compiler->parser = parser_create(compiler);
    if(!compiler->parser) {
        compiler_out_of_memory(compiler);
        return;
    }
}

void complier_destruct(s_compiler* compiler) {
    if(!compiler) return;
    parser_release(compiler->parser);
    compiler->parser = NULL;
}

void compiler_release(s_compiler* compiler) {
    complier_destruct(compiler);
    free(compiler);
}

void compiler_subtree_compile(s_compiler* compiler, s_tree_node** node_link) {
    s_tree_node* node = *node_link;
    if(node->left) compiler_subtree_compile(compiler, &node->left);
    if(node->right) compiler_subtree_compile(compiler, &node->right);

    if(node->type == TREE_NODE_TYPE_VARIABLE) {
        s_tree_node_variable* variable = (s_tree_node_variable*) node;

        int variable_name = tree_variable_by_string(variable->variable_name, variable->name_length);
        if(variable_name != -1) variable->variable_index = variable_name;
        else {
            e_tree_function function_index = tree_function_by_string(variable->variable_name, variable->name_length);
            if(function_index == (e_tree_function)-1) {
                char* name = strndup(variable->variable_name, variable->name_length);
                compiler_error(compiler, "Undefined reference: %s", name);
                free(name);
                return;
            }
            tree_release_subtree(node);
            *node_link = (s_tree_node*)tree_create_node_function(function_index);
            node = *node_link;
            if(node == NULL) {
                compiler_out_of_memory(compiler);
                return;
            }
        }
    } else if(node->type == TREE_NODE_TYPE_OPERATOR) {
        s_tree_node_operator* operator = (s_tree_node_operator*) node;
        if(operator->operator_type == TREE_OPERATOR_TYPE_CALL) {
            if(node->left->type != TREE_NODE_TYPE_FUNCTION) {
                if(!tree_is_differential(node->left)) {
                    compiler_error(compiler, "Expression cannot be called");
                    return;
                }
            }
        }
    }
}

void compiler_compile(s_compiler* compiler, s_tree* expression) {
    compiler->state = COMPILER_OK;
    compiler_subtree_compile(compiler, &expression->root);
}

int tree_variable_by_string(const char* string, unsigned long length) {
    for(int variable = 0; TREE_CONSTANT_NAMES[variable]; variable++) {
        if(strncmp(TREE_CONSTANT_NAMES[variable], string, length) == 0) return variable;
    }
    return -1;
}

e_tree_function tree_function_by_string(const char* string, unsigned long length) {
    for(e_tree_function function = 0; TREE_FUNCTION_NAMES[function]; function++) {
        if(strncmp(TREE_FUNCTION_NAMES[function], string, length) == 0) return function;
    }
    return -1;
}