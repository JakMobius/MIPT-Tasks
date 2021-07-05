//
// Created by Темыч on 05.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_COLORIZER_STATE_POP_CONTEXT_HPP
#define BONK_COMPILER_REINVENTED_X86_COLORIZER_STATE_POP_CONTEXT_HPP

#include "../x86_abstract_register.hpp"
#include "../../../../utils/hashmap.hpp"

namespace bonk::x86_backend {

struct colorizer_state_pop_context;
struct register_colorizer;

enum state_register_restore_stage {
    RESTORE_STAGE_MEM_REG,
    RESTORE_STAGE_REG_REG,
    RESTORE_STAGE_REG_MEM
};

}

namespace bonk::x86_backend {

struct colorizer_state_pop_context {
    register_colorizer* colorizer;
    int states_to_pop;

    hash_table<abstract_register, abstract_register_descriptor> target_transform;

    bool construct(register_colorizer* the_colorizer, int the_states_to_pop);
    bool count_transform();
    void destruct();

    bool should_save_register_with_owner(command_list* owner);

    abstract_register_descriptor get_current_descriptor(abstract_register reg);
    abstract_register_descriptor get_target_descriptor(abstract_register reg);

    void restore_registers();

    void restore_register_state(abstract_register reg, abstract_register_descriptor old_state, state_register_restore_stage stage);

    void walk_register_location_graph(bool* is_walked, abstract_register i);

    void try_restore_unowned_register_position(abstract_register i);

    void restore_register_from_memory(abstract_register reg, e_machine_register target);

    void restore_register_from_symbol(abstract_register reg, e_machine_register aRegister);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_COLORIZER_STATE_POP_CONTEXT_HPP
