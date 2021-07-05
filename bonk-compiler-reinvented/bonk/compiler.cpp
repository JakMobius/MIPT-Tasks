
#include "compiler.hpp"

namespace bonk {

void compiler::out_of_memory() {
    fatal_error("Compiler ran out of memory");
}

compiler* compiler::create(compiler_config* the_config) {
    auto* new_compiler = (compiler*)calloc(1, sizeof(compiler));
    if(!new_compiler) return nullptr;
    new_compiler->construct(the_config);
    return new_compiler;
}

void compiler::construct(compiler_config* the_config) {
    config = the_config;
    lexical_analyzer = lexical_analyzer::create(this);
    parser = parser::create(this);

    if(!parser || !lexical_analyzer) {
        out_of_memory();
        destruct();
        return;
    }
}

void compiler::destruct() {
    if(parser) parser->release();
    if(lexical_analyzer) lexical_analyzer->release();

    config = nullptr;
    lexical_analyzer = nullptr;
    parser = nullptr;
}

void compiler::release() {
    destruct();
    free(this);
}

tree_node_list<tree_node*>* compiler::get_ast_of_file_at_path(const char* file_path) {
    file_op_result file_read_result = FILE_OP_READ_ERROR;
    unsigned long file_length = 0;
    const char* real_path = realpath(file_path, nullptr);
    const char* file = read_file(real_path, &file_read_result, &file_length);

    if(file_read_result == FILE_OP_NOT_ENOUGH_MEMORY) {
        fatal_error("out of memory");
        return nullptr;
    } else if(file_read_result != FILE_OP_OK || real_path == nullptr) {
        fatal_error("failed to read input file\n");
        return nullptr;
    }

    auto* lexemes = lexical_analyzer->parse_file(real_path, file);
    free((void*) real_path);
    free((void*) file);

    if(!lexemes) {
        return nullptr;
    }

    return parser->parse_file(lexemes);
}

bool compiler::compile_ast(tree_node_list<tree_node*>* ast, FILE* target) {
    if(!config->compile_backend) {
        fatal_error("backend has not been specified");
        return false;
    }

    config->compile_backend->compile_ast(this, ast, target);

    return false;
}

}