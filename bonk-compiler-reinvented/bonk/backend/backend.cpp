//
// Created by Темыч on 21.04.2021.
//

#include "backend.hpp"

namespace bonk {

void backend::release() {
    destruct();
    free(this);
}

void backend::virtual_destruct() {

}

bool backend::virtual_compile_ast(compiler* linked_compiler, tree_node_list<tree_node*>* ast, FILE* target) {
    printf("abstract method compile_ast was not implemented");
    abort();
}

void backend::destruct() {
    (*this.*vtable->destruct)();
}

void backend::compile_ast(compiler* linked_compiler, tree_node_list<tree_node*>* ast, FILE* target) {
    (*this.*(vtable->compile_ast))(linked_compiler, ast, target);
}

const backend_method_table backend::methods = {
    .compile_ast = &backend::virtual_compile_ast,
    .destruct = &backend::virtual_destruct
};

}