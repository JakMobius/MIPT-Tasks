
#include "x86_field_list.hpp"

namespace bonk::x86_backend {

field_list* field_list::create(register_descriptor_list* descriptor_list, backend_context_state* state) {
    DEFAULT_ALLOC_CONSTRUCT(field_list, descriptor_list, state);
}

void field_list::construct(register_descriptor_list* descriptor_list, backend_context_state* state) {
    variables.construct(4);
    descriptors = descriptor_list;
    context_state = state;
    arguments = 0;
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
    command_list* list = nullptr;
    if(context_state) list = context_state->current_command_list;
//    if(variable->is_contextual) {
//        if(arguments < SYSTEM_V_ARGUMENT_REGISTERS_COUNT) {
//            variable->storage = descriptors->next_constrained_register(SYSTEM_V_ARGUMENT_REGISTERS[arguments], list);
//            arguments++;
//        } else {
//            variable->storage = descriptors->next_register(list);
//        }
//    } else {
        variable->storage = descriptors->next_register(list);
//    }
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