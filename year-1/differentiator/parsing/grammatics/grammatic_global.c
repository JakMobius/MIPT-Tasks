/*
 * Global grammatic:
 * $GLOBAL := (($EXPRESSION)[';'|'\n'])* '\0'
 */

#include "grammatic_global.h"

s_tree_node* parse_grammatic_global(s_parser* parser) {
    s_tree_node* result = NULL;
    while(true) {
        if(result) free(result);
        result = parse_grammatic_expression(parser);
        char next = parser_next_char(parser);
        if(next != ';' && next != '\n' && next != '\0') {
            free(result);
            parser_error(parser, "expected end of file");
            return NULL;
        }
        if(next == '\0') break;
        parser_eat_char(parser);
    }
    return result;
}