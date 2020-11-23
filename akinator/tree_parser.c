#include "tree_parser.h"

void tree_parser_previous_char(s_tree_parser* parser) {
    if(parser->text_index != 0) parser->text_index--;
}

char tree_parser_char_ahead(s_tree_parser* parser) {
    return parser->text[parser->text_index];
}

char tree_parser_next_char(s_tree_parser* parser) {
    char c = tree_parser_char_ahead(parser);
    if(c == '\0') return '\0';
    parser->text_index++;
    return c;
}

void tree_parser_read_until_char(s_tree_parser* parser, char expect) {
    char c;

    while((c = tree_parser_next_char(parser))) {
        if(c == expect) return;
    }
}

char* tree_parser_read_string_literal(s_tree_parser* parser) {

    tree_parser_read_until_char(parser, '"');

    unsigned long position = parser->text_index;

    tree_parser_read_until_char(parser, '"');

    unsigned long string_length = parser->text_index-position - 1;

    char* buffer = malloc(string_length + 1);
    memcpy(buffer, parser->text + position, string_length);
    buffer[string_length] = '\0';

    return buffer;
}

e_tree_error tree_parser_read_subtree(s_tree_parser* parser, s_tree_node** node) {
    e_tree_error error = tree_get_free_node(parser->target, node);
    if(error) return error;

    (*node)->value = tree_parser_read_string_literal(parser);

    char c;

    while((c = tree_parser_next_char(parser))) {
        if(c == '\"' || c == ']') {
            tree_parser_previous_char(parser);
            return TREE_OK;
        } else if(c == '[') {
            error = tree_parser_read_subtree(parser, &((*node)->left));
            if(error) return error;

            error = tree_parser_read_subtree(parser, &((*node)->right));
            if(error) return error;

            tree_parser_read_until_char(parser, ']');
        }
    }

    return TREE_OK;
}

e_tree_error tree_parser_read_tree(s_tree* target, const char* text) {
    if(!target) return TREE_ERROR_NULL;
    if(target->root) return TREE_ERROR_NOT_EMPTY;

    s_tree_parser parser = {0};
    parser.target = target;
    parser.text = text;
    parser.text_index = 0;

    e_tree_error error = tree_parser_read_subtree(&parser, &parser.target->root);

    return error;
}

e_tree_error tree_parser_serialize_subtree(s_tree_node* target, FILE* file, int offset) {
    if(target == NULL) return TREE_ERROR_NULL;
    if(file == NULL) return TREE_ERROR_NULL;

    for(int i = 0; i < offset; i++) fputc('\t', file);
    fprintf(file,"\"%s\"\n", target->value);

    if(target->left == NULL) return TREE_OK;

    for(int i = 0; i < offset; i++) fputc('\t', file);
    fprintf(file, "[\n");

    tree_parser_serialize_subtree(target->left, file, offset + 1);
    tree_parser_serialize_subtree(target->right, file, offset + 1);

    for(int i = 0; i < offset; i++) fputc('\t', file);
    fprintf(file, "]\n");

    return TREE_OK;
}

e_tree_error tree_parser_serialize(s_tree* target, FILE* file) {
    if(target == NULL) return TREE_ERROR_NULL;
    if(file == NULL) return TREE_ERROR_NULL;

    return tree_parser_serialize_subtree(target->root, file, 0);
}

e_tree_error tree_parser_dump_subtree(s_tree_node* target, FILE* file, int* node_counter) {
    if(target == NULL) return TREE_ERROR_NULL;
    if(file == NULL) return TREE_ERROR_NULL;

    int parent_id = *node_counter;
    fprintf(file,"\tnode_%d [label=\"%s\"]\n", *node_counter, target->value);

    if(target->left == NULL) return TREE_OK;

    (*node_counter)++;
    int left_id = *node_counter;

    tree_parser_dump_subtree(target->left, file, node_counter);

    fprintf(file, "\tnode_%d -> node_%d[label=yes]\n", parent_id, left_id);

    (*node_counter)++;
    int right_id = *node_counter;

    tree_parser_dump_subtree(target->right, file, node_counter);
    fprintf(file, "\tnode_%d -> node_%d[label=no]\n", parent_id, right_id);

    return TREE_OK;
}

e_tree_error tree_parser_dump(s_tree* target, FILE* file) {
    if(target == NULL) return TREE_ERROR_NULL;
    if(file == NULL) return TREE_ERROR_NULL;

    int node_counter = 0;

    fprintf(file, "digraph tree {\n");

    e_tree_error error = tree_parser_dump_subtree(target->root, file, &node_counter);

    fprintf(file, "}");

    return error;
}