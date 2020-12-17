/*
 * Global grammatic:
 * $GLOBAL := ($PROGRAM) '\0'
 */

#include "bonk_grammatic_global.h"

s_tree_node* bonk_parse_grammatic_global(s_bonk_parser* parser) {
    s_tree_node* result = NULL;

    result = (s_tree_node*) bonk_parse_grammatic_program(parser);
    if(!result) {
        if(!parser->compiler->state) bonk_parser_error(parser, "empty program");
        return NULL;
    }

    s_bonk_lexeme* next = bonk_parser_next_lexeme(parser);

    if(next->type != BONK_LEXEME_NULL) {
        tree_release_subtree(result);

        if(!parser->compiler->state) bonk_parser_error(parser, "expected end of file");

        return NULL;
    }

    return result;
}