
#include "x86_backend_context.hpp"
#include "x86_final_optimizer.hpp"

namespace bonk::x86_backend {

const e_machine_register SYSTEM_V_ARGUMENT_REGISTERS[] = {
    rdi, rsi, rdx, rcx, r8, r9
};
const int SYSTEM_V_ARGUMENT_REGISTERS_COUNT = sizeof(SYSTEM_V_ARGUMENT_REGISTERS) / sizeof(e_machine_register);

const e_machine_register SYSTEM_V_CALLEE_PRESERVED_REGISTERS[] = {
    // rbp - we preserve it explicitly
    rbx, r12, r13, r14, r15
};
const int SYSTEM_V_CALLEE_PRESERVED_REGISTERS_COUNT = sizeof(SYSTEM_V_CALLEE_PRESERVED_REGISTERS) / sizeof(e_machine_register);

const e_machine_register SYSTEM_V_CALLER_PRESERVED_REGISTERS[] = {
    rax, rcx, rdx, rsi, rdi, r8, r9, r10, r11
};
const int SYSTEM_V_CALLER_PRESERVED_REGISTERS_COUNT = sizeof(SYSTEM_V_CALLER_PRESERVED_REGISTERS) / sizeof(e_machine_register);

backend_context* backend_context::create(compiler* compiler, macho::macho_file* destination) {
    CHECK_ALLOC_CONSTRUCT(backend_context, compiler, destination)
}

bool backend_context::construct(compiler* compiler, macho::macho_file* destination) {
    linked_compiler = compiler;
    target = destination;

    scope_stack = scope_stack::create();

    if(!scope_stack) return false;
    if(state_stack.construct()) return false;

    global_descriptors = register_descriptor_list::create();
    if(!global_descriptors) return false;
    current_descriptors = global_descriptors;
    procedure_return_register = -1;
    procedure_body_container = nullptr;
    procedure_parameters = 0;

    return true;
}

void backend_context::destruct() {
    if(scope_stack) {
        scope_stack->release();
        scope_stack = nullptr;
    }
    if(global_descriptors) {
        global_descriptors->release();
        global_descriptors = nullptr;
    }
    state_stack.destruct();

    current_descriptors = nullptr;
}

void backend_context::release() {
    destruct();
    free(this);
}

void backend_context::compile_program(tree_node_list<tree_node*>* ast) {

    field_list* scope = read_scope_variables(ast);

    if(!linked_compiler->state) {

        mlist<tree_node*>* list = &ast->list;

        for(auto i = ast->list.begin(); i != ast->list.end(); ast->list.next_iterator(&i)) {
            auto* next_node = ast->list.get(i);

            if(next_node->type == TREE_NODE_TYPE_BLOCK_DEFINITION) {
                write_block_definition((tree_node_block_definition*) next_node);
            } else if(next_node->type == TREE_NODE_TYPE_VAR_DEFINITION) {
                write_global_var_definition((tree_node_variable_definition*) next_node);
            }
        }

        int total_global_variables = 0;
        for(auto i = list->begin(); i != list->end(); list->next_iterator(&i)) {
            auto* next_node = ast->list.get(i);

            if(next_node->type == TREE_NODE_TYPE_VAR_DEFINITION) {
                auto* var_definition = (tree_node_variable_definition*) next_node;
                if(var_definition->variable_value != nullptr) {
                    linked_compiler->error_positioned(var_definition->variable_value->source_position, "variables may not have initial value in x86 mode");
                }
                target->add_internal_symbol(var_definition->variable_name->variable_name, macho::SYMBOL_SECTION_DATA, total_global_variables * 8);
                auto storage = dynamic_array<char>::create(8);
                for(int j = 0; j < 8; j++) {
                    storage->push(0);
                }
                target->add_data(storage);
                total_global_variables++;
            }
        }

        for(int i = 0; i < scope->variables.size; i++) {
            variable* var = scope->variables.get(i);
            if(var->type == VARIABLE_TYPE_NUMBER) {
                auto descriptor = global_descriptors->get_descriptor(var->storage);
                descriptor->has_symbol_position = true;
                descriptor->located_in_symbol = true;
                descriptor->symbol_position = target->get_symbol_from_name(var->identifier->variable_name);
            }
        }

        for(auto i = ast->list.begin(); i != ast->list.end(); ast->list.next_iterator(&i)) {
            auto* next_node = ast->list.get(i);

            if(next_node->type == TREE_NODE_TYPE_BLOCK_DEFINITION) {
                auto* definition = (tree_node_block_definition*) next_node;
                if(definition->is_promise) {
                    write_block_promise(definition);
                } else {
                    write_block_implementation(definition);
                }
            }
        }
    }

    if(scope) {
        scope_stack->pop_scope();
    }

}

variable* backend_context::field_list_declare_variable(tree_node_variable_definition* node) {

    variable* var = scope_stack->get_variable(node->variable_name, nullptr);

    if(var == nullptr) {
        var = variable_number::create(node);

        field_list* top_scope = scope_stack->top();

        if(!var) linked_compiler->out_of_memory();
        else top_scope->add_variable(var);
        return var;
    } else {
        error_already_defined(node->variable_name);
    }

    return nullptr;
}

field_list* backend_context::read_scope_variables(tree_node_list<tree_node*>* node) {
    field_list* scope = field_list::create(current_descriptors, state);
    if(!scope || !scope_stack->push_scope(scope)) {
        linked_compiler->out_of_memory();
        return nullptr;
    }

    mlist<tree_node*>* list = &node->list;

    for(auto i = list->begin(); i != list->end(); list->next_iterator(&i)) {
        auto* next_node = node->list.get(i);

        tree_node_type node_type = next_node->type;
        if(node_type == TREE_NODE_TYPE_VAR_DEFINITION) {
            auto definition = (tree_node_variable_definition*) next_node;
            auto new_variable = field_list_declare_variable(definition);
            if(new_variable != nullptr) {
                if(definition->is_contextual) {
                    locate_procedure_parameter(new_variable);
                }
            }
        } else if(node_type == TREE_NODE_TYPE_BLOCK_DEFINITION) {
            field_list_declare_block((tree_node_block_definition*)next_node);
        }

        if(linked_compiler->state) {
            return nullptr;
        }
    }

    return scope;
}

field_list* backend_context::field_list_find_block_parameters(tree_node_block_definition* block) {
    field_list* argument_list = field_list::create(current_descriptors, state);
    if(!argument_list) return nullptr;

    mlist<tree_node*>* list = &block->body->list;

    for(auto i = list->begin(); i != list->end(); list->next_iterator(&i)) {
        auto* next_node = list->get(i);

        tree_node_type node_type = next_node->type;
        if(node_type != TREE_NODE_TYPE_VAR_DEFINITION) break;

        auto* var_definition = (tree_node_variable_definition*) next_node;

        if(!var_definition->is_contextual) break;

        variable* number = variable_number::create(var_definition);

        if(!number || !argument_list->add_variable(number)) {
            linked_compiler->out_of_memory();
            argument_list->release();
            return nullptr;
        }
    }

    return argument_list;
}

void backend_context::field_list_declare_block(tree_node_block_definition* node) {
    auto* identifier = node->block_name;

    variable* var = scope_stack->get_variable(identifier, nullptr);

    if(var == nullptr) {
        field_list* argument_list = field_list_find_block_parameters(node);
        variable_function* func = nullptr;

        if(argument_list) func = variable_function::create(identifier, argument_list);

        field_list* top_scope = scope_stack->top();

        if(!func || !top_scope->add_variable((variable*) func)) linked_compiler->out_of_memory();
    } else {
        error_already_defined(identifier);
    }
}

void backend_context::compile_block(tree_node_list<tree_node*>* block) {
    field_list* scope = read_scope_variables(block);

    if(!linked_compiler->state) {
        mlist<tree_node*>* list = &block->list;

        for(auto i = list->begin(); i != list->end(); list->next_iterator(&i)) {
            auto* next_node = list->get(i);
            compile_line(next_node);
        }
    }

    if(scope) {
        scope_stack->pop_scope();
    }
}

void backend_context::preserve_callee_registers() {
    state->current_command_list->insert_tail(reg_preserve_command::create(
        SYSTEM_V_CALLEE_PRESERVED_REGISTERS,
        SYSTEM_V_CALLEE_PRESERVED_REGISTERS_COUNT,
        state->current_command_list
    ));
}

void backend_context::compile_block_definition(tree_node_block_definition* block) {

    procedure_parameters = 0;
    procedure_return_register = procedure_command_buffer->descriptors.next_constrained_register(rax, state->current_command_list);

    push_state();

    procedure_body_container = state->current_command_list;

    preserve_callee_registers();
    state->current_command_list->insert_tail(frame_create_command::create(state->current_command_list));

    // As function body is a block which may use
    // caller-preserved registers, we should create
    // a compiler state.

    push_state();

    compile_block(block->body);

    pop_state();

    state->current_command_list->insert_tail(frame_destroy_command::create(state->current_command_list));
    state->current_command_list->insert_tail(ret_command::create(state->current_command_list));
    preserve_callee_registers();

    pop_state();
}

void backend_context::write_block_promise(tree_node_block_definition* block) {

}

void backend_context::write_block_implementation(tree_node_block_definition* block) {
    procedure_command_buffer = command_buffer::create(global_descriptors, target);
    current_descriptors = &procedure_command_buffer->descriptors;

    push_initial_state();
    compile_block_definition(block);
    pop_initial_state();

    // procedure_command_buffer->dump(stdout);

    auto colorized_buffer = register_colorizer::colorize(procedure_command_buffer);

    final_optimizer::optimize(colorized_buffer);
    if(linked_compiler->config->listing_file) {
        fprintf(linked_compiler->config->listing_file, "; == function ==\n%s:\n", block->block_name->variable_name);
        colorized_buffer->dump(linked_compiler->config->listing_file);
    }
    colorized_buffer->write_block_to_object_file(block->block_name->variable_name, target);

    colorized_buffer->release();
    procedure_command_buffer->release();
    current_descriptors = global_descriptors;
}

void backend_context::compile_plus(tree_node_operator* expression) {
    if(expression->left) compile_expression(expression->left);
    else state->register_stack.push_imm64(0);
    compile_expression(expression->right);
    state->register_stack.add();
}

void backend_context::compile_minus(tree_node_operator* expression) {
    if(expression->left) compile_expression(expression->left);
    else state->register_stack.push_imm64(0);
    compile_expression(expression->right);
    state->register_stack.sub();
}

void backend_context::compile_multiply(tree_node_operator* expression) {
    compile_expression(expression->left);
    compile_expression(expression->right);
    state->register_stack.mul();
}

void backend_context::compile_divide(tree_node_operator* expression) {
    compile_expression(expression->left);
    compile_expression(expression->right);
    state->register_stack.div();
}

void backend_context::compile_line(tree_node* node) {
    if(node->type == TREE_NODE_TYPE_VAR_DEFINITION) {
        auto* var_definition = (tree_node_variable_definition*) node;
        if(var_definition->variable_value) compile_assignment(var_definition->variable_name, var_definition->variable_value);
        else return;
    } else if(node->type == TREE_NODE_TYPE_CYCLE) {
        compile_cycle((tree_node_cycle*)node);
    } else if(node->type == TREE_NODE_TYPE_CHECK) {
        compile_check((tree_node_check*)node);
    } else {
        compile_expression(node);
    }
}

void backend_context::compile_expression(tree_node* node) {
    if(node->type == TREE_NODE_TYPE_OPERATOR) {
        auto* oper = (tree_node_operator*) node;

        switch(oper->oper_type) {
            case BONK_OPERATOR_ASSIGNMENT:            compile_assignment((tree_node_identifier*)oper->left, oper->right); break;
            case BONK_OPERATOR_PLUS:                  compile_plus(oper);                  break;
            case BONK_OPERATOR_MINUS:                 compile_minus(oper);                 break;
            case BONK_OPERATOR_MULTIPLY:              compile_multiply(oper);              break;
            case BONK_OPERATOR_DIVIDE:                compile_divide(oper);                break;
            case BONK_OPERATOR_OR:                    compile_or(oper);                    break;
            case BONK_OPERATOR_AND:                   compile_and(oper);                   break;
            case BONK_OPERATOR_EQUALS:                compile_equals(oper);                break;
            case BONK_OPERATOR_LESS_THAN:             compile_less_than(oper);             break;
            case BONK_OPERATOR_LESS_OR_EQUAL_THAN:    compile_less_or_equal_than(oper);    break;
            case BONK_OPERATOR_GREATER_THAN:          compile_greater_than(oper);          break;
            case BONK_OPERATOR_GREATER_OR_EQUAL_THAN: compile_greater_or_equal_than(oper); break;
            case BONK_OPERATOR_NOT_EQUAL:             compile_not_equal(oper);             break;
            case BONK_OPERATOR_BREK:                  compile_brek_statement(oper);        break;
            case BONK_OPERATOR_REBONK:                compile_rebonk_statement(oper);      break;
            case BONK_OPERATOR_BONK:                  compile_bonk_statement(oper);        break;
//            case BONK_OPERATOR_PRINT:      compile_print           (oper);                           break;
//            case BONK_OPERATOR_BAMS:       compile_inline_assembly (oper);                           break;
//            default:                       compile_math_comparsion (oper);                           break;
        }
    } else if(node->type == TREE_NODE_TYPE_CALL) {
        compile_call((tree_node_call*)node);
    } else if(node->type == TREE_NODE_TYPE_NUMBER) {
        state->register_stack.push_imm64(((tree_node_number*)node)->integer_value);
    } else if(node->type == TREE_NODE_TYPE_IDENTIFIER) {
        abstract_register reg = get_variable((tree_node_identifier*)node);
        if(reg == -1) return;
        state->register_stack.push_reg64(reg);
    }
}

void backend_context::compile_assignment(tree_node_identifier* variable, tree_node* value) {
    compile_expression(value);
    abstract_register reg = get_variable(variable);
    if(reg == -1) return;
    
    state->register_stack.write_head(reg);
}

abstract_register backend_context::get_variable(tree_node_identifier* identifier) {
    field_list* scope = nullptr;
    variable* variable = scope_stack->get_variable(identifier, &scope);

    if(variable == nullptr) {
        error_undefined_reference(identifier);
        return -1;
    }

    if(variable->type == VARIABLE_TYPE_FUNCTION) {
        linked_compiler->error_positioned(identifier->source_position, "'%s' is a block name, not variable", identifier->variable_name);
        return -1;
    }

    return variable->storage;
}

void backend_context::push_state() {
    auto new_command_list = procedure_command_buffer->next_command_list();
    state_stack.push({});
    state = state_stack.point(state_stack.size - 1);
    state->construct(new_command_list);
}

void backend_context::pop_state() {
    auto previous_state = state_stack.point(state_stack.size - 2);
    auto* command = scope_command::create(state->current_command_list, previous_state->current_command_list);
    state->destruct();
    state_stack.pop();
    state = previous_state;
    state->current_command_list->insert_tail(command);
}

void backend_context::push_initial_state() {
    state_stack.push({});
    state = state_stack.point(state_stack.size - 1);
    state->construct(procedure_command_buffer->root_list);
}

void backend_context::pop_initial_state() {
    state->destruct();
    state_stack.pop();
    state = nullptr;
    procedure_body_container = nullptr;
}

void backend_context::compile_cycle(tree_node_cycle* cycle) {

    auto old_cycle_container = cycle_container;
    auto old_cycle_body = cycle_body;
    auto old_cycle_head = cycle_head;
    auto old_cycle_tail = cycle_tail;

    cycle_head = create_label();
    cycle_tail = create_label();

    cycle_container = state->current_command_list;
    state->current_command_list->insert_tail(cycle_head);

    push_state();
    compile_block(cycle->body);
    state->current_command_list->insert_tail(scope_repeat_command::create(state->current_command_list));
    pop_state();

    state->current_command_list->insert_tail(jmp_command::create(cycle_head, state->current_command_list));
    state->current_command_list->insert_tail(cycle_tail);

    cycle_container = old_cycle_container;
    cycle_body = old_cycle_body;
    cycle_head = old_cycle_head;
    cycle_tail = old_cycle_tail;
}

void backend_context::error_already_defined(tree_node_identifier* identifier) {
    parser_position* position = identifier->source_position;
    linked_compiler->error_positioned(position, "variable '%s' is already defined", identifier->variable_name);
}

void backend_context::error_undefined_reference(tree_node_identifier* node) {
    parser_position* position = node->source_position;
    linked_compiler->error_positioned(position, "undefined reference: '%s'", node->variable_name);
}

void backend_context::pop_to_scope(command_list* scope) {
    for(int i = state_stack.size - 1, pop = 0;; i--, pop++) {
        if(scope == state_stack.point(i)->current_command_list) {
            state->current_command_list->insert_tail(scope_pop_command::create(pop, state->current_command_list));
            return;
        }
        assert(i != 0);
    }
}

void backend_context::compile_bonk_statement(tree_node_operator* oper) {
    if(oper->right) {
        compile_expression(oper->right);
        state->register_stack.pop(procedure_return_register);
    } else {
        state->current_command_list->insert_tail(xor_command::create_reg64_reg64(procedure_return_register, procedure_return_register, state->current_command_list));
    }

    pop_to_scope(procedure_body_container);

    state->current_command_list->insert_tail(frame_destroy_command::create(state->current_command_list));
    state->current_command_list->insert_tail(ret_command::create(state->current_command_list, procedure_return_register));
    state->current_command_list->insert_tail(scope_dead_end_command::create(state->current_command_list));
}

bool backend_context::can_use_fast_logic(tree_node_operator* oper) {
    return can_use_fast_logic_on_operand(oper->left, oper->oper_type) &&
           can_use_fast_logic_on_operand(oper->right, oper->oper_type);
}

bool backend_context::can_use_fast_logic_on_operand(tree_node* operand, operator_type oper_type) {
    if(operand->type == TREE_NODE_TYPE_OPERATOR){
        if(((tree_node_operator*)operand)->oper_type == oper_type) {
            return can_use_fast_logic((tree_node_operator*)operand);
        }
    }
    return operand->type == TREE_NODE_TYPE_NUMBER || operand->type == TREE_NODE_TYPE_IDENTIFIER;
}

bool backend_context::compile_logic_operand_recursive(tree_node* node, jmp_label* control_label, operator_type oper_type, bool check_for) {
    auto guard = compile_boolean_jump(node, check_for);
    if(!guard) return false;
    pop_to_scope(control_label->list);
    state->current_command_list->insert_tail(jmp_command::create(control_label, state->current_command_list));
    guard->set_label(insert_label());
    return true;
}

bool backend_context::compile_logic_recursive(tree_node_operator* oper, jmp_label* control_label, operator_type oper_type, bool check_for) {
    if(oper->left->type == TREE_NODE_TYPE_OPERATOR && ((tree_node_operator*) oper->left)->oper_type == oper_type) {
        // left-hand operand must be expression
        assert(compile_logic_recursive((tree_node_operator*)(oper->left), control_label, oper_type, check_for));
    } else {
        // left-hand operand must be expression
        assert(compile_logic_operand_recursive(oper->left, control_label, oper_type, check_for));
    }

    if(oper->right->type == TREE_NODE_TYPE_OPERATOR && ((tree_node_operator*) oper->right)->oper_type == oper_type) {
        return compile_logic_recursive((tree_node_operator*)(oper->right), control_label, oper_type, check_for);
    } else {
        return compile_logic_operand_recursive(oper->right, control_label, oper_type, check_for);
    }
}

void backend_context::compile_jump_logical(tree_node_operator* oper) {
    bool is_and = oper->oper_type == BONK_OPERATOR_AND;
    auto control_label = create_label();
    auto flag = state->register_stack.push_placeholder(true);

    push_state();
    bool is_expression = compile_logic_recursive(oper, control_label, oper->oper_type, is_and);
    pop_state();

    if(is_expression) {
        auto end_label = create_label();
        push_state();
        state->current_command_list->insert_tail(mov_command::create_reg8_imm8(flag, is_and, state->current_command_list));
        pop_to_scope(end_label->list);
        state->current_command_list->insert_tail(jmp_command::create(end_label, state->current_command_list));
        state->current_command_list->insert_tail(scope_dead_end_command::create(state->current_command_list));
        pop_state();

        state->current_command_list->insert_tail(control_label);

        push_state();
        state->current_command_list->insert_tail(mov_command::create_reg8_imm8(flag, !is_and, state->current_command_list));
        pop_state();

        state->current_command_list->insert_tail(end_label);
    } else {
        state->current_command_list->insert_tail(control_label);
    }
}

void backend_context::compile_or(tree_node_operator* oper) {
    if(can_use_fast_logic(oper)) {
        compile_expression(oper->left);
        compile_expression(oper->right);
        state->register_stack.logical_or();
    } else {
        compile_jump_logical(oper);
    }
}

void backend_context::compile_and(tree_node_operator* oper) {
    if(can_use_fast_logic(oper)) {
        compile_expression(oper->left);
        compile_expression(oper->right);
        state->register_stack.logical_and();
    } else {
        compile_jump_logical(oper);
    }
}

void backend_context::compile_equals(tree_node_operator* oper) {
    compile_expression(oper->left);
    compile_expression(oper->right);
    state->register_stack.equals();
}

void backend_context::compile_less_than(tree_node_operator* oper) {
    compile_expression(oper->left);
    compile_expression(oper->right);
    state->register_stack.less_than();
}

void backend_context::compile_less_or_equal_than(tree_node_operator* oper) {
    compile_expression(oper->left);
    compile_expression(oper->right);
    state->register_stack.less_or_equal_than();
}

void backend_context::compile_greater_than(tree_node_operator* oper) {
    compile_expression(oper->left);
    compile_expression(oper->right);
    state->register_stack.greater_than();
}

void backend_context::compile_greater_or_equal_than(tree_node_operator* oper) {
    compile_expression(oper->left);
    compile_expression(oper->right);
    state->register_stack.greater_or_equal_than();
}

void backend_context::compile_not_equal(tree_node_operator* oper) {
    compile_expression(oper->left);
    compile_expression(oper->right);
    state->register_stack.not_equal();
}

general_jmp_command* backend_context::append_oper_jmp_command(tree_node_operator* oper, bool inversed) {
    compile_expression(oper->left);
    compile_expression(oper->right);
    state->register_stack.compare();
    general_jmp_command* jmp = nullptr;
    switch(oper->oper_type) {
        case BONK_OPERATOR_EQUALS:                jmp = je_command::create(nullptr, state->current_command_list); break;
        case BONK_OPERATOR_LESS_THAN:             jmp = jl_command::create(nullptr, state->current_command_list); break;
        case BONK_OPERATOR_LESS_OR_EQUAL_THAN:    jmp = jng_command::create(nullptr, state->current_command_list); break;
        case BONK_OPERATOR_GREATER_THAN:          jmp = jg_command::create(nullptr, state->current_command_list); break;
        case BONK_OPERATOR_GREATER_OR_EQUAL_THAN: jmp = jge_command::create(nullptr, state->current_command_list); break;
        case BONK_OPERATOR_NOT_EQUAL:             jmp = jne_command::create(nullptr, state->current_command_list); break;
        default: assert(false);
    }

    if(inversed) {
        jmp->invert_condition();
    }
    state->current_command_list->insert_tail(jmp);
    return jmp;
}

general_jmp_command* backend_context::compile_boolean_jump(tree_node* node, bool check_for) {
    general_jmp_command* jmp = nullptr;

    if(node->type == TREE_NODE_TYPE_OPERATOR) {
        auto oper = (tree_node_operator*)node;
        switch(oper->oper_type) {
            case BONK_OPERATOR_EQUALS:
            case BONK_OPERATOR_LESS_THAN:
            case BONK_OPERATOR_LESS_OR_EQUAL_THAN:
            case BONK_OPERATOR_GREATER_THAN:
            case BONK_OPERATOR_GREATER_OR_EQUAL_THAN:
            case BONK_OPERATOR_NOT_EQUAL:
                jmp = append_oper_jmp_command(oper, !check_for);
                break;
            default: break;
        }
    }

    if(!jmp) {
        long long old_size = state->register_stack.stack.size;
        compile_expression(node);
        if(state->register_stack.stack.size == old_size) return nullptr;
        state->register_stack.test();
        if(check_for) {
            jmp = jne_command::create(nullptr, state->current_command_list);
        } else {
            jmp = je_command::create(nullptr, state->current_command_list);
        }
        state->current_command_list->insert_tail(jmp);
    }

    return jmp;
}

// I'm proud of how natural this code look like
void backend_context::compile_check(tree_node_check* node) {

    auto jmp = compile_boolean_jump(node->condition, false);

    push_state();
    compile_block(node->check_body);
    pop_state();

    jmp->set_label(insert_label());

}

void backend_context::compile_brek_statement(tree_node_operator* oper) {
    if(!cycle_container) {
        linked_compiler->error_positioned(oper->source_position, "brek operator outside of cycle");
        return;
    }

    pop_to_scope(cycle_container);
    state->current_command_list->insert_tail(jmp_command::create(cycle_tail, state->current_command_list));
}

void backend_context::compile_rebonk_statement(tree_node_operator* oper) {
    if(!cycle_body) {
        linked_compiler->error_positioned(oper->source_position, "rebonk operator outside of cycle");
        return;
    }

    pop_to_scope(cycle_body);
    state->current_command_list->insert_tail(jmp_command::create(cycle_head, state->current_command_list));
}

jmp_label* backend_context::create_label() {
    return jmp_label::create(state->current_command_list);
}

jmp_label* backend_context::insert_label() {
    auto nop = jmp_label::create(state->current_command_list);
    state->current_command_list->insert_tail(nop);
    return nop;
}

tree_node* backend_context::call_argument_list_get_value(tree_node_list<tree_node_call_parameter*>* argument_list, tree_node_identifier* identifier) {
    if(!argument_list) return nullptr;

    mlist<tree_node_call_parameter*>* list = &argument_list->list;

    for(auto i = list->begin(); i != list->end(); list->next_iterator(&i)) {
        auto* parameter = list->get(i);

        if(parameter->parameter_name->contents_equal(identifier)) {
            return parameter->parameter_value;
        }
    }

    return nullptr;
}

tree_node* backend_context::compile_nth_argument(variable_function* func, tree_node_list<tree_node_call_parameter*>* argument_list, int i) {
    variable* argument = func->argument_list->variables.get(i);

    tree_node* value = call_argument_list_get_value(argument_list, argument->identifier);
    if(value == nullptr) {
        parser_position* position = func->identifier->source_position;
        linked_compiler->error_positioned(position, "'%s' requires contextual variable '%s'", func->identifier->variable_name, argument->identifier->variable_name);
        return nullptr;
    }

    int stack_size = state->register_stack.stack.size;
    compile_expression(value);
    if(state->register_stack.stack.size == stack_size) {
        linked_compiler->error_positioned(value->source_position, "expression does not return anything");
    }

    return value;
}

void backend_context::compile_call(tree_node_call* call) {
    auto* argument_list = call->call_parameters;
    auto* function_name = (tree_node_identifier*) call->call_function;

    variable* var = scope_stack->get_variable(function_name, nullptr);

    if(var == nullptr) {
        error_undefined_reference(function_name);
        return;
    }

    if(var->type != VARIABLE_TYPE_FUNCTION) {
        parser_position* position = function_name->source_position;
        linked_compiler->error_positioned(position, "'%s' is not a function", function_name->variable_name);
        return;
    }

    auto* func = (variable_function*) var;
    field_list* function_arguments = func->argument_list;

    // rdi rsi rdx rcx r8 r9 stack

    int register_arguments = function_arguments->variables.size;
    if(register_arguments > SYSTEM_V_ARGUMENT_REGISTERS_COUNT) {
        register_arguments = SYSTEM_V_ARGUMENT_REGISTERS_COUNT;
    }
    int stack_arguments = function_arguments->variables.size - register_arguments;

    state->current_command_list->insert_tail(
            align_stack_command::create_before(stack_arguments, state->current_command_list));

    for(int i = function_arguments->variables.size - 1; i >= SYSTEM_V_ARGUMENT_REGISTERS_COUNT; i--) {
        auto value = compile_nth_argument(func, argument_list, i);
        if(!value) return;

        abstract_register reg = state->register_stack.get_head_register_number();
        state->register_stack.decrease_stack_size();
        state->current_command_list->insert_tail(push_command::create_reg64(reg, state->current_command_list));
    }

    abstract_register arguments[SYSTEM_V_ARGUMENT_REGISTERS_COUNT] = {};

    for(int i = 0; i < SYSTEM_V_ARGUMENT_REGISTERS_COUNT && i < function_arguments->variables.size; i++) {
        auto value = compile_nth_argument(func, argument_list, i);
        if(!value) return;

        abstract_register reg = procedure_command_buffer->descriptors.next_constrained_register(SYSTEM_V_ARGUMENT_REGISTERS[i], state->current_command_list);
        arguments[i] = reg;
        state->register_stack.pop(reg);
    }

    state->current_command_list->insert_tail(call_command::create(
        command_parameter_symbol::create(
            true,
            target->get_symbol_from_name(call->call_function->variable_name)
        ),
        arguments,
        register_arguments,
        state->current_command_list
    ));

    state->current_command_list->insert_tail(align_stack_command::create_after(stack_arguments, state->current_command_list));
    state->register_stack.push_reg64(procedure_command_buffer->descriptors.next_constrained_register(rax, state->current_command_list));

}

void backend_context::write_block_definition(tree_node_block_definition* definition) {
    if(definition->is_promise) {
        target->declare_external_symbol(definition->block_name->variable_name);
    } else {
        target->declare_internal_symbol(definition->block_name->variable_name);
    }
}

void backend_context::write_global_var_definition(tree_node_variable_definition* definition) {
    target->declare_internal_symbol(definition->variable_name->variable_name);
}

void backend_context::locate_procedure_parameter(variable* parameter) {
    if(procedure_parameters < SYSTEM_V_ARGUMENT_REGISTERS_COUNT) {
        e_machine_register reg = SYSTEM_V_ARGUMENT_REGISTERS[procedure_parameters];
        state->current_command_list->insert_tail(locate_reg_command::create_reg64_reg64(parameter->storage, reg, state->current_command_list));
    } else {
        int state_position = (procedure_parameters - SYSTEM_V_ARGUMENT_REGISTERS_COUNT + 2) * -8;
        state->current_command_list->insert_tail(locate_reg_command::create_reg64_stack(parameter->storage, state_position, state->current_command_list));
    }

    procedure_parameters++;
}

}