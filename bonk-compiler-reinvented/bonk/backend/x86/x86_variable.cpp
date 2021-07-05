//
// Created by Темыч on 23.04.2021.
//

#include "x86_variable.hpp"

namespace bonk::x86_backend {

void variable::construct(tree_node_identifier* the_identifier) {
    identifier = the_identifier;
}

void variable::destroy() {

}

void variable::release() {
    destroy();
    free(this);
}

variable_number* variable_number::create(tree_node_variable_definition* definition) {
    auto* number = (variable_number*)calloc(1, sizeof(variable));

    number->construct(definition);

    return number;
}

void variable_number::construct(tree_node_variable_definition* definition) {
    ((variable*) this)->construct(definition->variable_name);
    this->is_contextual = definition->is_contextual;
    type = VARIABLE_TYPE_NUMBER;
}

variable_function* variable_function::create(tree_node_identifier* the_identifier, field_list* the_argument_list) {
    auto* function = (variable_function*)calloc(1, sizeof(variable_function));

    function->construct(the_identifier, the_argument_list);

    return function;
}

void variable_function::construct(tree_node_identifier* the_identifier, field_list* the_argument_list) {
    ((variable*) this)->construct(the_identifier);
    argument_list = the_argument_list;
    type = VARIABLE_TYPE_FUNCTION;
}

}