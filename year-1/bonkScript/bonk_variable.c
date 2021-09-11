
#include "bonk_variable.h"

void bonk_variable_construct(s_bonk_variable* variable, s_tree_node_identifier* identifier) {
    variable->identifier = identifier;
}

s_bonk_variable* bonk_variable_number_create(s_tree_node_identifier* identifier) {
    s_bonk_variable* number = calloc(1, sizeof(s_bonk_variable));

    bonk_variable_construct(number, identifier);
    bonk_variable_number_construct(number);

    return number;
}

void bonk_variable_number_construct(s_bonk_variable* number) {
    number->type = BONK_VARIABLE_TYPE_NUMBER;
}

s_bonk_variable_function* bonk_variable_function_create(s_tree_node_identifier* identifier, s_bonk_field_list* argument_list) {
    s_bonk_variable_function* function = calloc(1, sizeof(s_bonk_variable_function));

    bonk_variable_construct((s_bonk_variable*) function, identifier);
    bonk_variable_function_construct(function, argument_list);

    return function;
}

void bonk_variable_function_construct(s_bonk_variable_function* function, s_bonk_field_list* argument_list) {
    function->super_structure.type = BONK_VARIABLE_TYPE_FUNCTION;
    function->argument_list = argument_list;
}

void bonk_variable_destroy(s_bonk_variable* variable) {

}

void bonk_variable_release(s_bonk_variable* variable) {
    bonk_variable_destroy(variable);
    free(variable);
}