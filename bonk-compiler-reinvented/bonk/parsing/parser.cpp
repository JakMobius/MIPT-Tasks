
#include "parser.hpp"

namespace bonk {

parser* parser::create(compiler* compiler) {
    DEFAULT_ALLOC_CONSTRUCT(parser, compiler);
}

void parser::construct(compiler* compiler) {
    linked_compiler = compiler;
    input = nullptr;
}

void parser::destruct() {

}

void parser::release() {
    destruct();
    free(this);
}

tree_node_list<tree_node*>* parser::parse_file(dynamic_array<lexeme>* lexemes) {
    input = lexemes;
    auto* target = tree_node_list<tree_node*>::create();
    if(!target) return nullptr;

    if(!parse_grammatic_global(this, target)) {
        target->release();
        return nullptr;
    }

    return target;
}

bool parser::append_file(dynamic_array<lexeme>* lexemes, tree_node_list<tree_node*>* target) {
    input = lexemes;

    return parse_grammatic_global(this, target);
}

void parser::spit_lexeme() {
    position--;
}

lexeme* parser::next_lexeme() {
    return input->point((int)position);
}

void parser::eat_lexeme() {
    lexeme* c = next_lexeme();
    assert(c->type);
    position++;
}



}