

#include "bonk_grammatic_help.h"

s_tree_node* bonk_parse_grammatic_help(s_bonk_parser* parser) {
    s_bonk_lexeme* next = bonk_parser_next_lexeme(parser);
    if(next->type == BONK_LEXEME_KEYWORD && next->keyword_data.keyword_type == BONK_KEYWORD_HELP) {
        bonk_parser_eat_lexeme(parser);

        next = bonk_parser_next_lexeme(parser);
        if(next->type != BONK_LEXEME_IDENTIFIER) {
            bonk_parser_error(parser, "expected library or file name to import");
            return NULL;
        }
        bonk_parser_eat_lexeme(parser);

        const char* library_storage = "/usr/local/lib/bonkScript/help/";
        const char* library_name = tree_node_identifier_name(next->identifier_data.identifier);
        const char* library_extension = ".bs";

        char* full_path = malloc(strlen(library_storage) + strlen(library_name) + strlen(library_extension) + 1);

        int length = sprintf(full_path, "%s%s%s", library_storage, library_name, library_extension);

        full_path[length] = '\0';

        file_op_result read_result = FILE_OP_OK;
        const char* source = read_file(full_path, &read_result, NULL);

        if(read_result == FILE_OP_NOT_ENOUGH_MEMORY) {
            free(full_path);
            free((void*)library_name);
            compiler_out_of_memory(parser->compiler);
            return NULL;
        } else if(read_result != FILE_OP_OK) {
            free(full_path);
            bonk_parser_error(parser, "failed to open '%s': '%s'", library_name, strerror(errno));
            free((void*)library_name);
            return NULL;
        }

        free((void*)library_name);

        s_bonk_lexeme* lexeme = bonk_lexical_analyzer_parse_file(parser->compiler->lexical_analyzer, full_path, source);
        s_tree_node* result = NULL;

        if(!parser->compiler->state && lexeme) {
            s_bonk_parser* nested_parser = bonk_parser_create(parser->compiler);

            if(!nested_parser) {
                free(full_path);
                compiler_out_of_memory(parser->compiler);
                return NULL;
            }

            s_tree* tree = bonk_parser_parse_file(nested_parser, lexeme);
            result = tree->root;
            tree->root = NULL;
            tree_release(tree);

            bonk_parser_release(nested_parser);
        }

        // TODO: store all files in compiler structure

        //free((void*)source);

        return result;
    }

    return NULL;
}