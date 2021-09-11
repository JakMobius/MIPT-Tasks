
#include "ede_field_list.hpp"

namespace bonk::ede_backend {

field_list* field_list::create() {
    auto* new_scope = (field_list*)calloc(1, sizeof(field_list));
    if(!new_scope) return nullptr;
    new_scope->construct();
    return new_scope;
}

void field_list::construct() {
    variables.construct(4);
    byte_offset = 0;
}

variable* field_list::get_variable(tree_node_identifier* identifier) {

    for(int i = 0; i < variables.size; i++) {
        if(variables.get(i)->identifier->contents_equal(identifier)) {
            return variables.get(i);
        }
    }

    return nullptr;
}

bool field_list::add_variable(variable* variable) {
    variable->byte_offset = frame_size + byte_offset;

    frame_size += 8;
    if(variables.push(variable)) return false;

    return true;
}

void field_list::destroy() {
    for(int i = 0; i < variables.size; i++) {
        variables.get(i)->release();
    }
    variables.destruct();
}

void field_list::release() {
    destroy();
    free(this);
}

}