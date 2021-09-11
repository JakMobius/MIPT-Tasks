
#include "x86_backend.hpp"
#include "x86_backend_context.hpp"

namespace bonk::x86_backend {

backend* backend::create() {
    CHECK_ALLOC_CONSTRUCT(backend);
}

bool backend::construct() {
    vtable = &methods;
    return true;
}

void backend::destruct() {
    (*this.*vtable->destruct)();
}

void backend::release() {
    destruct();
    free(this);
}

void backend::virtual_destruct() {

}

bool backend::virtual_compile_ast(compiler* linked_compiler, tree_node_list<bonk::tree_node*>* ast, FILE* target) {

    backend_context context = {};
    macho::macho_file* file = macho::macho_file::create();

    if(!context.construct(linked_compiler, file)) {
        linked_compiler->out_of_memory();
        return false;
    }
    context.compile_program(ast);

    /*
     * TODO: get rid of global compiler state in
     * order to make it possible to do parallel
     * compilation
     */

    context.destruct();

    if(linked_compiler->state) {
        linked_compiler->state = BONK_COMPILER_OK;
        return false;
    }

    file->flush(target);
    file->release();

    return true;
}

const backend_method_table backend::methods = inherit(bonk::backend::methods, backend_method_table {
    .compile_ast = (bool (bonk::backend::*)(compiler*, tree_node_list<bonk::tree_node*> *, FILE *)) &backend::virtual_compile_ast,
    .destruct = (void (bonk::backend::*)()) &backend::virtual_destruct
});

}