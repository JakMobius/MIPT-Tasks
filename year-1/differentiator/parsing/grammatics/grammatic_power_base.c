
#include "grammatic_power_base.h"

s_tree_node* parse_grammatic_power_base(s_parser* parser) {
    s_tree_node* result = NULL;

    unsigned long old_position = parser->position.index;
    result = parse_grammatic_reference(parser);
    if(!result && old_position != parser->position.index) return NULL;
    if(result) return result;

    old_position = parser->position.index;
    result = parse_grammatic_number(parser);
    if(!result && old_position != parser->position.index) return NULL;
    if(result) return result;

    return result;
}