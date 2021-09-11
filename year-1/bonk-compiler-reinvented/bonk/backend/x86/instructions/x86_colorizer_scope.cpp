
#include "x86_colorizer_scope.hpp"

namespace bonk::x86_backend {

scope_command* scope_command::create(command_list* nested_list, command_list* list) {

    scope_command* result = nullptr;

    avl_tree<abstract_register> read_registers_tree = {};
    avl_tree<abstract_register> write_registers_tree = {};

    read_registers_tree.construct();
    write_registers_tree.construct();

    if(nested_list->append_read_register(&read_registers_tree) && nested_list->append_write_register(&write_registers_tree)) {
        result = (scope_command*) allocate_structure(0, read_registers_tree.size, write_registers_tree.size, sizeof(scope_command), &list->parent_buffer->allocator);
        if(result) {
            result->list = list;
            result->construct(nested_list, &read_registers_tree, &write_registers_tree);
        }
    }

    read_registers_tree.destruct();
    write_registers_tree.destruct();

    return result;
}

// Passing read_registers and write_registers as parameters here
// to not recalculate these arrays

void scope_command::construct(command_list* list, avl_tree<abstract_register>* read_registers, avl_tree<abstract_register>* write_registers) {
    type = COMMAND_COLORIZER_SCOPE;
    int read_index = 0;
    int write_index = 0;

    commands = list;

    for(auto i = commands->begin(); i != commands->end(); commands->next_iterator(&i)) {
        asm_command* command = commands->get(i);
        for(int j = 0; j < command->total_read_registers; j++) {
            auto read_reg = command->get_read_register(j);
            if(read_registers->erase(read_reg)) {
                set_read_register(read_index++, read_reg);
            }
        }
        for(int j = 0; j < command->total_write_registers; j++) {
            auto write_reg = command->get_write_register(j);
            if(write_registers->erase(write_reg)) {
                set_write_register(write_index++, write_reg);
            }
        }
    }
}

void scope_command::dump(FILE* file, int depth) {
    depth_padding(file, depth, false);
    fprintf(file, "colorizer::scope {\n");
    if(total_read_registers) {
        depth_padding(file, depth + 1, false);
        fprintf(file, "; %d read registers (", total_read_registers);
        for(int i = 0; i < total_read_registers; i++) {
            if(i > 0) fprintf(file, ", ");
            dump_register(file, get_read_register(i));
        }
        fprintf(file, ")\n");
    }
    if(total_write_registers) {
        depth_padding(file, depth + 1, false);
        fprintf(file, "; %d write registers (", total_write_registers);
        for(int i = 0; i < total_write_registers; i++) {
            if(i > 0) fprintf(file, ", ");
            dump_register(file, get_write_register(i));
        }
        fprintf(file, ")\n");
    }
    commands->dump(file, depth + 1);
    depth_padding(file, depth, false);
    printf("}");
}
}