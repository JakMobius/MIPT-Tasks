/*
 * Global grammatic:
 * $GLOBAL := ($PROGRAM) '\0'
 */

#include "bonk_grammatic_global.h"

s_tree_node* bonk_parse_grammatic_global(s_bonk_parser* parser) {
    s_tree_node* result = NULL;
    s_tree_node* list = NULL;

    while(true) {
        unsigned long old_position = parser->position;
        s_tree_node* import = bonk_parse_grammatic_help(parser);
        if(!import) {
            if(old_position == parser->position) break;
            continue;
        }

        if(list == NULL) {
            result = import;
        } else {
            list->right = import;
        }

        while(import->right) import = import->right;

        list = import;
    }


    s_tree_node* program = (s_tree_node*) bonk_parse_grammatic_program(parser);
    if(!program) {
        if(!parser->compiler->state) bonk_parser_error(parser, "empty program");
        return NULL;
    }

    if(list) {
        list->right = program;
    } else {
        result = program;
    }

    s_bonk_lexeme* next = bonk_parser_next_lexeme(parser);

    if(next->type != BONK_LEXEME_NULL) {
        tree_release_subtree(result);

        if(!parser->compiler->state) bonk_parser_error(parser, "expected end of file");

        return NULL;
    }

    return result;
}