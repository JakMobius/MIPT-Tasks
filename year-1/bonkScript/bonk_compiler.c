
#include "bonk_compiler.h"

void compiler_out_of_memory(s_bonk_compiler* compiler) {
    bonk_compiler_fatal_error(compiler, "Compiler ran out of memory");
}

s_bonk_compiler* bonk_compiler_create(s_bonk_compiler_config* config) {
    s_bonk_compiler* compiler = calloc(1, sizeof(s_bonk_compiler));
    bonk_compiler_construct(compiler, config);
    return compiler;
}

void bonk_compiler_construct(s_bonk_compiler* compiler, s_bonk_compiler_config* config) {
    compiler->config = config;
    compiler->lexical_analyzer = bonk_lexical_analyzer_create(compiler);
    compiler->parser = bonk_parser_create(compiler);
    compiler->scope_stack = bonk_scope_stack_create();
    compiler->labels = 1;
    compiler->cycle_label_id = 0;
    compiler->compiled_files = NULL;
    compiler->compiled_files_amount = 0;
    compiler->compiled_files_buffer_length = 0;

    if(!compiler->parser || !compiler->lexical_analyzer || !compiler->scope_stack) {
        compiler_out_of_memory(compiler);
        return;
    }
}

void bonk_compiler_destruct(s_bonk_compiler* compiler) {
    if(!compiler) return;
    bonk_parser_release(compiler->parser);
    compiler->parser = NULL;
}

void bonk_compiler_release(s_bonk_compiler* compiler) {
    bonk_compiler_destruct(compiler);
    free(compiler);
}

void bonk_compiler_error_undefined_reference(s_bonk_compiler* compiler, s_tree_node_identifier* node) {
    const char* variable_name = tree_node_identifier_name(node);
    s_bonk_parser_position* position = node->tree_node.source_position;
    bonk_compiler_error_positioned(compiler, position, "undefined reference: '%s'", variable_name);
    free((void*)variable_name);
}

void bonk_compiler_reference_variable(s_bonk_compiler* compiler, s_tree_node_identifier* identifier) {
    s_bonk_field_list* scope = NULL;
    s_bonk_variable* variable = bonk_scope_stack_get_variable(compiler->scope_stack, identifier, &scope);

    if(variable == NULL) {
        bonk_compiler_error_undefined_reference(compiler, identifier);
        return;
    }

    if(variable->type == BONK_VARIABLE_TYPE_FUNCTION) {
        const char* variable_name = tree_node_identifier_name(identifier);
        bonk_compiler_error_positioned(compiler, identifier->tree_node.source_position, "'%s' is a block name, not variable", variable_name);
        free((void*)variable_name);
        return;
    }

    if(scope == compiler->scope_stack->scopes[0]) {
        fprintf(compiler->config->target_file, "[0x%lx]", variable->byte_offset);
    } else {
        if(variable->byte_offset == compiler->scope_stack_offset) {
            fprintf(compiler->config->target_file, "[rdx]");
        } else if(variable->byte_offset > compiler->scope_stack_offset){
            fprintf(compiler->config->target_file, "[rdx + 0x%lx]", variable->byte_offset - compiler->scope_stack_offset);
        } else {
            fprintf(compiler->config->target_file, "[rdx - 0x%lx]", compiler->scope_stack_offset - variable->byte_offset);
        }
    }

    fprintf(compiler->config->target_file, " ; referencing '");
    bonk_print_identifier(compiler->config->target_file, variable->identifier);
    fprintf(compiler->config->target_file, "' field");
}

void bonk_compile_factor(s_bonk_compiler* compiler, s_tree_node* node) {
    if(node->type == TREE_NODE_TYPE_NUMBER) {
        fprintf(compiler->config->target_file, "push.d %.20LG\n", ((s_tree_node_number*)node)->float_value);
        compiler->processor_stack_usage += 8;
    } else if(node->type == TREE_NODE_TYPE_IDENTIFIER) {
        fprintf(compiler->config->target_file, "push.d ");
        bonk_compiler_reference_variable(compiler, (s_tree_node_identifier*)node);
        fprintf(compiler->config->target_file, "\n");
        compiler->processor_stack_usage += 8;
    } else if(node->type == TREE_NODE_TYPE_OPERATOR) {
        bonk_compile_expression(compiler, node, 8);
    }
}

void bonk_compile_term(s_bonk_compiler* compiler, s_tree_node* node) {
    if(node->type == TREE_NODE_TYPE_OPERATOR) {
        s_tree_node_operator* operator = (s_tree_node_operator*) node;
        e_bonk_operator_type operator_type = operator->operator_type;

        if(operator_type == BONK_OPERATOR_MULTIPLY || operator_type == BONK_OPERATOR_DIVIDE) {
            bonk_compile_term(compiler, node->left);
            bonk_compile_term(compiler, node->right);

            if(operator_type == BONK_OPERATOR_MULTIPLY) {
                fprintf(compiler->config->target_file, "mul.d\n");
                compiler->processor_stack_usage -= 8;
            } else {
                fprintf(compiler->config->target_file, "div.d\n");
                compiler->processor_stack_usage -= 8;
            }
            return;
        }
    }

    bonk_compile_factor(compiler, node);
}

void bonk_compile_math_expression(s_bonk_compiler* compiler, s_tree_node* node) {
    if(node->type == TREE_NODE_TYPE_OPERATOR) {
        s_tree_node_operator* operator = (s_tree_node_operator*) node;
        e_bonk_operator_type operator_type = operator->operator_type;

        if(operator_type == BONK_OPERATOR_PLUS || operator_type == BONK_OPERATOR_MINUS) {
            if(node->left) {
                bonk_compile_math_expression(compiler, node->left);
            } else {
                fprintf(compiler->config->target_file, "push.d 0\n");
                compiler->processor_stack_usage += 8;
            }
            bonk_compile_math_expression(compiler, node->right);

            if(operator_type == BONK_OPERATOR_PLUS) {
                fprintf(compiler->config->target_file, "add.d\n");
                compiler->processor_stack_usage -= 8;
            } else {
                fprintf(compiler->config->target_file, "sub.d\n");
                compiler->processor_stack_usage -= 8;
            }
            return;
        }
    }

    bonk_compile_term(compiler, node);
}

const char* bonk_compiler_get_comparsion_instruction(e_bonk_operator_type operator_type) {
    switch(operator_type) {
        case BONK_OPERATOR_EQUALS: return "je";
        case BONK_OPERATOR_LESS_OR_EQUAL_THAN: return "jbe";
        case BONK_OPERATOR_GREATER_OR_EQUAL_THAN: return "jae";
        case BONK_OPERATOR_LESS_THAN: return "jb";
        case BONK_OPERATOR_GREATER_THAN: return "ja";
        case BONK_OPERATOR_NOT_EQUAL: return "jne";
        default: return NULL;
    }
}

const char* bonk_compiler_get_comparsion_instruction_negated(e_bonk_operator_type operator_type) {
    switch(operator_type) {
        case BONK_OPERATOR_EQUALS: return "jne";
        case BONK_OPERATOR_LESS_OR_EQUAL_THAN: return "ja";
        case BONK_OPERATOR_GREATER_OR_EQUAL_THAN: return "jb";
        case BONK_OPERATOR_LESS_THAN: return "jae";
        case BONK_OPERATOR_GREATER_THAN: return "jbe";
        case BONK_OPERATOR_NOT_EQUAL: return "je";
        default: return NULL;
    }
}

void bonk_compile_math_comparsion(s_bonk_compiler* compiler, s_tree_node* node) {
    if(node->type == TREE_NODE_TYPE_OPERATOR) {
        s_tree_node_operator* operator = (s_tree_node_operator*) node;
        e_bonk_operator_type operator_type = operator->operator_type;

        const char* comparing_instruction = bonk_compiler_get_comparsion_instruction(operator_type);

        if(comparing_instruction != NULL) {
            bonk_compile_math_comparsion(compiler, node->left);
            bonk_compile_math_comparsion(compiler, node->right);

            unsigned long label_id = bonk_compiler_next_label_id(compiler);

            fprintf(compiler->config->target_file, "%s.d _cmp_%lu_ok\n"
                                                   "push.d 0\n"
                                                   "jmp _cmp_%lu_end\n"
                                                   "_cmp_%lu_ok:\n"
                                                   "push.d 1\n"
                                                   "_cmp_%lu_end:\n", comparing_instruction, label_id, label_id, label_id, label_id);
            compiler->processor_stack_usage -= 8;
            return;
        }
    }

    bonk_compile_math_expression(compiler, node);
}

s_tree_node* bonk_call_argument_list_get_value(s_bonk_compiler* compiler, s_tree_node* argument_list, s_tree_node_identifier* identifier) {
    s_tree_node* argument_list_walker = argument_list;
    if(!argument_list) return NULL;

    while(true) {
        if(argument_list_walker->left) {
            if(tree_node_identifier_equals((s_tree_node_identifier*) argument_list_walker->left->left, identifier)) {
                return argument_list_walker->left->right;
            }
        }

        if(argument_list_walker->right) argument_list_walker = argument_list_walker->right;
        else return NULL;
    }
}

void bonk_compiler_write_call(s_bonk_compiler* compiler, s_tree_node* call) {
    s_tree_node* argument_list = call->right;
    s_tree_node_identifier* function_name = (s_tree_node_identifier*) call->left;

    s_bonk_variable* variable = bonk_scope_stack_get_variable(compiler->scope_stack, function_name, NULL);

    if(variable == NULL) {
        bonk_compiler_error_undefined_reference(compiler, function_name);
        return;
    }

    if(variable->type != BONK_VARIABLE_TYPE_FUNCTION) {
        const char* function_name_string = tree_node_identifier_name(function_name);
        s_bonk_parser_position* position = function_name->tree_node.source_position;
        bonk_compiler_error_positioned(compiler, position, "'%s' is not a function", function_name_string);
        free((void*)function_name_string);
        return;
    }

    s_bonk_variable_function* function = (s_bonk_variable_function*) variable;
    s_bonk_field_list* function_arguments = function->argument_list;
    s_bonk_field_list* top_scope = bonk_scope_stack_top(compiler->scope_stack);

    const char* function_name_string = tree_node_identifier_name(function_name);

    unsigned long safe_zone_offset = function_arguments->variables_amount * 8;

    fprintf(compiler->config->target_file, " ; calling block %s\n", function_name_string);
    fprintf(compiler->config->target_file, " ;  top frame size = %lu\n", top_scope->frame_size);
    fprintf(compiler->config->target_file, " ;  top frame offset = %lu\n", top_scope->byte_offset);
    fprintf(compiler->config->target_file, " ;  safe zone = %lu\n", safe_zone_offset);

    unsigned long argument_calculation_offset = top_scope->frame_size + top_scope->byte_offset + safe_zone_offset;

    if(argument_calculation_offset > 0) {
        fprintf(compiler->config->target_file, "push.ul (rdx + 0x%lx)\n"
                                               "pop.ul rdx\n",
                top_scope->frame_size + top_scope->byte_offset + safe_zone_offset);

        compiler->scope_stack_offset += top_scope->frame_size + top_scope->byte_offset + safe_zone_offset;
    }

    for(int i = 0; i < function_arguments->variables_amount; i++) {
        s_bonk_variable* argument = function_arguments->variables[i];

        s_tree_node* value = bonk_call_argument_list_get_value(compiler, argument_list, argument->identifier);
        if(value == NULL) {
            const char* missing_parameter_name = tree_node_identifier_name(argument->identifier);

            s_bonk_parser_position* position = function_name->tree_node.source_position;
            bonk_compiler_error_positioned(compiler, position, "'%s' requires contextual variable '%s'", function_name_string, missing_parameter_name);

            free((void*)missing_parameter_name);
            return;
        } else {
            bonk_compile_expression(compiler, value, 8);
            fprintf(compiler->config->target_file, "pop.d [rdx - 0x%lx] ; writing to argument '", safe_zone_offset - i * 8);
            bonk_print_identifier(compiler->config->target_file, argument->identifier);
            fprintf(compiler->config->target_file, "'\n");
            compiler->processor_stack_usage -= 8;
        }
    }

    compiler->scope_stack_offset -= top_scope->frame_size + top_scope->byte_offset + safe_zone_offset;

    if(safe_zone_offset > 0) {
        fprintf(compiler->config->target_file, "push.ul (rdx - 0x%lx)\n"
                                               "pop.ul rdx\n", safe_zone_offset);
    }

    fprintf(compiler->config->target_file, "call _bs_%s\n", function_name_string);

    if(top_scope->frame_size + top_scope->byte_offset > 0) {
        fprintf(compiler->config->target_file, "push.ul (rdx - 0x%lx)\n"
                                               "pop.ul rdx\n", top_scope->frame_size + top_scope->byte_offset);
    }

    compiler->processor_stack_usage += 8;

    free((void*)function_name_string);
}

void bonk_compile_inline_assembly(s_bonk_compiler* compiler, s_tree_node* node) {
    fprintf(compiler->config->target_file, "; inline assembly at line %lu:\n", node->source_position->line);

    s_tree_node_identifier* inline_asm = (s_tree_node_identifier*) node->left;
    s_tree_node_identifier inline_variable = {0};
    inline_variable.tree_node.source_position = node->source_position;

    const char* asm_begin = inline_asm->variable_name;
    unsigned long asm_length = inline_asm->name_length;

    bool was_whitespace = false;
    bool was_newline = true;

    while(asm_length) {
        char next = *(asm_begin++);
        asm_length--;

        if(next == ' ' || next == '\t') {
            if(was_whitespace || was_newline) continue;
            was_whitespace = true;
            was_newline = false;
            fputc(next, compiler->config->target_file);
            continue;
        }
        was_whitespace = false;

        if(next == '\n') {
            if(was_newline) continue;
            was_newline = true;
            fputc(next, compiler->config->target_file);
            continue;
        }
        was_newline = false;

        if(next == '%') {
            inline_variable.variable_name = asm_begin;
            inline_variable.name_length = -1;

            do {
                inline_variable.name_length ++;
                next = *(asm_begin++);
                asm_length--;
            } while(next != '%');

            if(inline_variable.name_length == 0) {
                bonk_compiler_error_positioned(compiler, node->source_position, "empty variable reference in inline assembly");
            } else {
                bonk_compiler_reference_variable(compiler, &inline_variable);
            }
            continue;
        }

        fputc(next, compiler->config->target_file);
    }

    if(!was_newline) fputc('\n', compiler->config->target_file);

    fprintf(compiler->config->target_file, "; inline assembly end:\n");
}

void bonk_compile_bonk_statement(s_bonk_compiler* compiler, s_tree_node* node, unsigned long stack_bytes_expected) {
    if(stack_bytes_expected > 0) {
        bonk_compiler_error_positioned(compiler, node->source_position, "bonk does not return anything");
    }

    bonk_compile_expression(compiler, node->right, 8);
    fprintf(compiler->config->target_file, "pop.d rbx\n"
                                           "pop.ul rax\n"
                                           "push.d rbx\n"
                                           "push.ul rax\n"
                                           "ret\n");
    compiler->processor_stack_usage -= 8;
}

void bonk_compile_brek_statement(s_bonk_compiler* compiler, s_tree_node* node, unsigned long stack_bytes_expected) {
    if(compiler -> cycle_label_id == 0) {
        bonk_compiler_error_positioned(compiler, node->source_position, "brek statement outside cycle");
    } else {
        if(stack_bytes_expected > 0) {
            bonk_compiler_error_positioned(compiler, node->source_position, "brek does not return anything");
        }
        fprintf(compiler->config->target_file, "jmp _cycle_%lu_end\n", compiler->cycle_label_id);
    }
}

void bonk_compile_rebonk_statement(s_bonk_compiler* compiler, s_tree_node* node, unsigned long stack_bytes_expected) {
    if(compiler -> cycle_label_id == 0) {
        bonk_compiler_error_positioned(compiler, node->source_position, "rebonk statement outside cycle");
    } else {
        if(stack_bytes_expected > 0) {
            bonk_compiler_error_positioned(compiler, node->source_position, "rebonk does not return anything");
        }
        fprintf(compiler->config->target_file, "jmp _cycle_%lu\n", compiler->cycle_label_id);
    }
}

void bonk_compile_assignment(s_bonk_compiler* compiler, s_tree_node* node, unsigned long stack_bytes_expected) {
    bonk_compile_expression(compiler, node->right, 8);
    if(stack_bytes_expected != 0) {
        fprintf(compiler->config->target_file, "pop.d rax\n"
                                               "push.d rax\n"
                                               "push.d rax\n");
        compiler->processor_stack_usage += 8;
    }
    fprintf(compiler->config->target_file, "pop.d ");
    bonk_compiler_reference_variable(compiler, (s_tree_node_identifier*)node->left);
    fprintf(compiler->config->target_file, "\n");
    compiler->processor_stack_usage -= 8;
}

void bonk_compile_cycle(s_bonk_compiler* compiler, s_tree_node* node, unsigned long stack_bytes_expected) {
    if(stack_bytes_expected > 0) {
        bonk_compiler_error_positioned(compiler, node->source_position, "cycle does not return anything");
    }

    unsigned long wrapping_cycle_id = compiler->cycle_label_id;
    unsigned long label_id = bonk_compiler_next_label_id(compiler);
    compiler->cycle_label_id = label_id;

    fprintf(compiler->config->target_file, "_cycle_%lu:\n", label_id);
    bonk_compile_block(compiler, node->right, false);
    fprintf(compiler->config->target_file, "jmp _cycle_%lu\n"
                                           "_cycle_%lu_end:\n", label_id, label_id);

    compiler->cycle_label_id = wrapping_cycle_id;
}

void bonk_compile_check(s_bonk_compiler* compiler, s_tree_node* node, unsigned long stack_bytes_expected) {
    if(stack_bytes_expected > 0) {
        bonk_compiler_error_positioned(compiler, node->source_position, "check does not return anything");
    }

    unsigned long label_id = bonk_compiler_next_label_id(compiler);

    const char* comparsion_instruction = NULL;

    s_tree_node* comparsion = node->left;

    if(comparsion->type == TREE_NODE_TYPE_OPERATOR) {
        comparsion_instruction = bonk_compiler_get_comparsion_instruction_negated(((s_tree_node_operator*)comparsion)->operator_type);
    }

    if(comparsion_instruction == NULL) {
        bonk_compile_expression(compiler, comparsion, 8);
        compiler->processor_stack_usage -= 8;
        comparsion_instruction = "jz";
    } else {
        bonk_compile_expression(compiler, comparsion->left, 8);
        bonk_compile_expression(compiler, comparsion->right, 8);
        compiler->processor_stack_usage -= 16;
    }

    if(node->right->right) {
        fprintf(compiler->config->target_file, "%s.d _if_%lu_else\n", comparsion_instruction, label_id);
        bonk_compile_block(compiler, node->right->left, false);
        fprintf(compiler->config->target_file, "jmp _if_%lu_end\n"
                                               "_if_%lu_else:\n", label_id, label_id);
        bonk_compile_block(compiler, node->right->right, false);
        fprintf(compiler->config->target_file, "_if_%lu_end:\n", label_id);
    } else {
        fprintf(compiler->config->target_file, "%s.d _if_%lu_end\n", comparsion_instruction, label_id);
        bonk_compile_block(compiler, node->right->left, false);
        fprintf(compiler->config->target_file, "_if_%lu_end:\n", label_id);
    }
}

void bonk_compile_logic(s_bonk_compiler* compiler, s_tree_node_operator* operator, unsigned long stack_bytes_expected) {
    unsigned long label_id = bonk_compiler_next_label_id(compiler);

    bonk_compile_expression(compiler, operator->tree_node.left, 8);

    if(stack_bytes_expected > 0) {
        fprintf(compiler->config->target_file, "pop.d rax\n"
                                               "push.d rax\n"
                                               "push.d rax\n");
    }

    if(operator->operator_type == BONK_OPERATOR_OR) {
        fprintf(compiler->config->target_file, "jnz.d _log_%lu_ok\n", label_id);
    } else {
        fprintf(compiler->config->target_file, "jz.d _log_%lu_ok\n", label_id);
    }

    if(stack_bytes_expected > 0) {
        fprintf(compiler->config->target_file, "push.ul (rsp - 0x8)\n"
                                               "pop.ul rsp\n");
    }

    compiler->processor_stack_usage -= 8;
    bonk_compile_expression(compiler, operator->tree_node.right, stack_bytes_expected);
    fprintf(compiler->config->target_file, "_log_%lu_ok:\n", label_id);
}

void bonk_compile_print(s_bonk_compiler* compiler, s_tree_node* node) {
    bonk_compile_expression(compiler, node->right, 8);
    fprintf(compiler->config->target_file, "out.d\n");
}

unsigned long bonk_compiler_next_label_id(s_bonk_compiler* compiler) {
    return compiler->labels++;
}

void bonk_compile_expression(s_bonk_compiler* compiler, s_tree_node* node, unsigned long stack_bytes_expected) {
    unsigned long saved_stack_usage = compiler->processor_stack_usage + stack_bytes_expected;

    if(node->type == TREE_NODE_TYPE_OPERATOR) {
        s_tree_node_operator* operator = (s_tree_node_operator*) node;

        switch(operator->operator_type) {
            case BONK_OPERATOR_PRINT:      bonk_compile_print           (compiler, node);                           break;
            case BONK_OPERATOR_BONK:       bonk_compile_bonk_statement  (compiler, node, stack_bytes_expected);     break;
            case BONK_OPERATOR_BREK:       bonk_compile_brek_statement  (compiler, node, stack_bytes_expected);     break;
            case BONK_OPERATOR_REBONK:     bonk_compile_rebonk_statement(compiler, node, stack_bytes_expected);     break;
            case BONK_OPERATOR_ASSIGNMENT: bonk_compile_assignment      (compiler, node, stack_bytes_expected);     break;
            case BONK_OPERATOR_CALL:       bonk_compiler_write_call     (compiler, node);                           break;
            case BONK_OPERATOR_CYCLE:      bonk_compile_cycle           (compiler, node, stack_bytes_expected);     break;
            case BONK_OPERATOR_CHECK:      bonk_compile_check           (compiler, node, stack_bytes_expected);     break;
            case BONK_OPERATOR_OR:
            case BONK_OPERATOR_AND:        bonk_compile_logic           (compiler, operator, stack_bytes_expected); break;
            case BONK_OPERATOR_BAMS:       bonk_compile_inline_assembly (compiler, node);                           break;
            default:                       bonk_compile_math_comparsion (compiler, node);                           break;
        }
    } else if(node->type == TREE_NODE_TYPE_VAR_DEFINITION) {
        if(node->right) bonk_compile_assignment(compiler, node, stack_bytes_expected);
        else return;
    } else {
        bonk_compile_math_expression(compiler, node);
    }

    unsigned long stack_bytes_to_pop = compiler->processor_stack_usage - saved_stack_usage;

    if(stack_bytes_to_pop > 0) {
        fprintf(compiler->config->target_file, "push.ul (rsp - %lu)\npop.ul rsp\n", stack_bytes_to_pop);
        compiler->processor_stack_usage = saved_stack_usage;
    }
}

void bonk_compiler_error_already_defined(s_bonk_compiler* compiler, s_tree_node_identifier* identifier) {
    const char* variable_name = tree_node_identifier_name(identifier);
    s_bonk_parser_position* position = identifier->tree_node.source_position;

    bonk_compiler_error_positioned(compiler, position, "variable '%s' is already defined", variable_name);
    free((void*)variable_name);
}

void bonk_field_list_declare_variable(s_bonk_compiler* compiler, s_tree_node* node) {
    s_tree_node_identifier* identifier = (s_tree_node_identifier*)node->left;

    s_bonk_variable* variable = bonk_scope_stack_get_variable(compiler->scope_stack, identifier, NULL);

    if(variable == NULL) {
        variable = bonk_variable_number_create(identifier);

        s_bonk_field_list* top_scope = bonk_scope_stack_top(compiler->scope_stack);

        if(!variable) compiler_out_of_memory(compiler);
        else bonk_field_list_add_variable(top_scope, variable);
    } else {
        bonk_compiler_error_already_defined(compiler, identifier);
    }
}

s_bonk_field_list* bonk_field_list_find_block_parameters(s_bonk_compiler* compiler, s_tree_node* block) {
    s_bonk_field_list* argument_list = bonk_field_list_create();
    if(!argument_list) return NULL;

    s_tree_node* expression_list = block;

    while(true) {
        s_tree_node* list_node = expression_list->left;

        if(list_node) {
            e_tree_node_type node_type = list_node->type;
            if(node_type != TREE_NODE_TYPE_VAR_DEFINITION) break;
            if(!((s_tree_node_variable_definition*) list_node)->is_contextual) break;

            s_bonk_variable* number = bonk_variable_number_create((s_tree_node_identifier*) list_node->left);

            if(!number || !bonk_field_list_add_variable(argument_list, number)) {
                compiler_out_of_memory(compiler);
                bonk_field_list_release(argument_list);
                return NULL;
            }
        }

        if(expression_list->right) expression_list = expression_list->right;
        else break;
    }

    return argument_list;
}

void bonk_field_list_declare_block(s_bonk_compiler* compiler, s_tree_node* node) {
    s_tree_node_identifier* identifier = (s_tree_node_identifier*)node->left;

    s_bonk_variable* variable = bonk_scope_stack_get_variable(compiler->scope_stack, identifier, NULL);

    if(variable == NULL) {
        s_bonk_field_list* argument_list = bonk_field_list_find_block_parameters(compiler, node->right);
        s_bonk_variable_function* function = NULL;

        if(argument_list) function = bonk_variable_function_create(identifier, argument_list);

        s_bonk_field_list* top_scope = bonk_scope_stack_top(compiler->scope_stack);

        if(!function) compiler_out_of_memory(compiler);
        else bonk_field_list_add_variable(top_scope, (s_bonk_variable*) function);
    } else {
        bonk_compiler_error_already_defined(compiler, identifier);
    }
}

s_bonk_field_list* bonk_read_scope_variables(s_bonk_compiler* compiler, s_tree_node* node, bool reset_frame_offset) {

    s_bonk_field_list* scope = bonk_field_list_create();
    if(!scope || !bonk_scope_stack_push_scope(compiler->scope_stack, scope)) {
        compiler_out_of_memory(compiler);
        return NULL;
    }

    if(reset_frame_offset) scope->byte_offset = 0;

    s_tree_node* expression_list = node;

    while(true) {
        s_tree_node* list_node = expression_list->left;

        if(list_node) {
            e_tree_node_type node_type = list_node->type;
            if(node_type == TREE_NODE_TYPE_VAR_DEFINITION) bonk_field_list_declare_variable(compiler, list_node);
            else if(node_type == TREE_NODE_TYPE_BLOCK_DEFINITION) bonk_field_list_declare_block(compiler, list_node);

            if(compiler->state) {
                return NULL;
            }
        }

        if(expression_list->right) expression_list = expression_list->right;
        else break;
    }

    return scope;
}

void bonk_compile_block(s_bonk_compiler* compiler, s_tree_node* node, bool reset_frame_offset) {
    s_bonk_field_list* scope = bonk_read_scope_variables(compiler, node, reset_frame_offset);

    if(!compiler->state) {
        s_tree_node* expression_list = node;

        while(true) {
            if(expression_list->left) bonk_compile_expression(compiler, expression_list->left, 0);
            if(expression_list->right) expression_list = expression_list->right;
            else break;
        }
    }

    if(scope) {
        bonk_scope_stack_pop_scope(compiler->scope_stack);
    }


}

void bonk_compile_callable_block(s_bonk_compiler* compiler, s_tree_node* node) {
    fprintf(compiler->config->target_file, "; block ");
    bonk_print_identifier(compiler->config->target_file, (s_tree_node_identifier*)node->left);
    fprintf(compiler->config->target_file, "\n");

    fprintf(compiler->config->target_file, "_bs_");
    bonk_print_identifier(compiler->config->target_file, (s_tree_node_identifier*)node->left);
    fprintf(compiler->config->target_file, ":\n");

    bonk_compile_block(compiler, node->right, true);

    fprintf(compiler->config->target_file, "pop.ul rax\n"
                                           "push.d 0\n"
                                           "push.ul rax\n"
                                           "ret\n");
}

void bonk_compile_program(s_bonk_compiler* compiler, s_tree_node* node) {

    s_tree_node* walker = node;

    s_bonk_field_list* scope = bonk_read_scope_variables(compiler, node, false);

    if(!compiler->state) {
        while(true) {
            if(walker->left && walker->left->type == TREE_NODE_TYPE_VAR_DEFINITION && walker->left->right) {
                bonk_compile_assignment(compiler, walker->left, 0);
            }

            if(walker->right) walker = walker->right;
            else break;
        }

        if(scope->frame_size > 0) {
            fprintf(compiler->config->target_file, "push.ul (rdx + 0x%lx)\n"
                                                   "pop.ul rdx\n", scope->frame_size);
        }

        fprintf(compiler->config->target_file, "call _bs_main\n");
        fprintf(compiler->config->target_file, "hlt\n");

        walker = node;

        while(true) {
            if(walker->left && walker->left->type == TREE_NODE_TYPE_BLOCK_DEFINITION) {
                bonk_compile_callable_block(compiler, walker->left);
            }

            if(walker->right) walker = walker->right;
            else break;
        }
    }

    if(scope) {
        bonk_scope_stack_pop_scope(compiler->scope_stack);
    }

    fprintf(compiler->config->target_file, "hlt\n");
}

bool bonk_compiler_resize_compiled_files_buffer(s_bonk_compiler* compiler, unsigned long new_capacity) {
    const char** new_buffer = realloc((void*)compiler->compiled_files, new_capacity * sizeof(const char*));

    if(new_buffer == NULL) {
        compiler_out_of_memory(compiler);
        return false;
    }

    compiler->compiled_files = new_buffer;
    return true;
}

bool bonk_compiler_add_compiled_file(s_bonk_compiler* compiler, const char* file_path) {
    if(compiler->compiled_files_amount == compiler->compiled_files_buffer_length) {
        unsigned long new_capacity = compiler->compiled_files_buffer_length * 2;
        if(new_capacity < 16) new_capacity = 16;
        if(!bonk_compiler_resize_compiled_files_buffer(compiler, new_capacity)) return false;
    }

    compiler->compiled_files[compiler->compiled_files_amount++] = file_path;
    return true;
}

bool bonk_compiler_file_already_compiled(s_bonk_compiler* compiler, const char* file_path) {
    for(int i = 0; i < compiler->compiled_files_amount; i++) {
        if(strcmp(compiler->compiled_files[i], file_path) == 0) return true;
    }
    return false;
}