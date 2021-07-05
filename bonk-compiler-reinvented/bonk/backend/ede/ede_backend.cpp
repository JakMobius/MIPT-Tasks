
#include "ede_backend.hpp"

namespace bonk::ede_backend {

backend* backend::create() {
    CHECK_ALLOC_CONSTRUCT(backend);
}

bool backend::construct() {
    vtable = &methods;
    return true;
}

bool backend::virtual_compile_ast(compiler* linked_compiler, tree_node_list<bonk::tree_node*>* ast, FILE* target) {

    backend_context context = {};

    context.construct(linked_compiler, target);
    context.compile_program(ast);
    context.destruct();

    if(linked_compiler->state) {
        linked_compiler->state = BONK_COMPILER_OK;
        return false;
    }

    return true;
}

const backend_method_table backend::methods = inherit(backend::methods, backend_method_table {
    .compile_ast = (bool (bonk::backend::*)(compiler*, tree_node_list<bonk::tree_node*> *, FILE *)) &backend::virtual_compile_ast,
    .destruct = &backend::virtual_destruct
});

}