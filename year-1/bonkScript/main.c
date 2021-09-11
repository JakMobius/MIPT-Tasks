#include <stdio.h>
#include <sys/stat.h>
#include "bonk_compiler.h"
#include "argv_parser.h"
#include "command_line_arguments.h"
#include "file_io.h"

void print_lexemes(s_bonk_lexeme* lexemes);

int main(int argc, const char* argv[]) {

    if(argc <= 1) {
        argv_print_help(ARGV_TYPES);
        return 0;
    }

    const char* input = argv[1];

    s_argv_view* arguments = argv_view_construct(argc, argv, ARGV_TYPES);

    if(!arguments) {
        compiler_out_of_memory(NULL);
        return -1;
    }

    s_arg_view* help_flag = argv_view_get_arg_view(arguments, &ARGV_TYPES[E_ARGV_NAME_HELP]);

    if(help_flag != NULL || input[0] == '-') {
        argv_print_help(ARGV_TYPES);
        return 0;
    }

    file_op_result read_result = FILE_OP_INVALID_ARGUMENT;
    unsigned long length = 0;

    const char* real_path = realpath(input, NULL);
    const char* file = read_file(real_path, &read_result, &length);

    s_arg_view* output_file_flag = argv_view_get_arg_view(arguments, &ARGV_TYPES[E_ARGV_NAME_OUTPUT_FILE]);

    const char* output_file_path = NULL;

    if(output_file_flag) output_file_path = output_file_flag -> value;
    else output_file_path = "out.ede";

    FILE* output_file = NULL;

    if(output_file_path != NULL) {
        output_file = fopen(output_file_path, "wb");

        if(!output_file) {
            bonk_compiler_fatal_error(NULL, "failed to open input file\n");
            return 1;
        }
    }

    if(read_result == FILE_OP_NOT_ENOUGH_MEMORY) {
        compiler_out_of_memory(NULL);
        return 1;
    } else if(read_result != FILE_OP_OK || real_path == NULL) {
        bonk_compiler_fatal_error(NULL, "failed to read input file\n");
        return 1;
    }

    s_bonk_compiler_config config = {0};
    config.target_file = output_file;
    config.error_file = stderr;

    s_bonk_compiler* compiler = bonk_compiler_create(&config);

    if(compiler->state) {
        return -1;
    }

    s_bonk_lexeme* lexemes = bonk_lexical_analyzer_parse_file(compiler->lexical_analyzer, real_path, file);

    if(!compiler->state) {
        s_tree* result = bonk_parser_parse_file(compiler->parser, lexemes);

        if(!compiler->state) {
            bonk_compile_program(compiler, result->root);
        }
    }

    fclose(output_file);

    if(chmod(output_file_path, 511) < 0) {
        bonk_compiler_warning(NULL, "failed to add execution permissions to file\n");
    }

    free((void*)real_path);
    free((void*)file);

    argv_view_destroy(arguments);

    if(compiler->state) {
        bonk_compiler_release(compiler);
        return 1;
    }

    bonk_compiler_release(compiler);
    return 0;
}


void print_lexemes(s_bonk_lexeme* lexemes) {
    for(int i = 0; lexemes[i].type; i++) {
        switch(lexemes[i].type) {
            case BONK_LEXEME_KEYWORD:
                printf("keyword: %s\n", BONK_KEYWORD_NAMES[lexemes[i].keyword_data.keyword_type]);
                break;
            case BONK_LEXEME_IDENTIFIER:
                printf("identifier: ");
                fwrite(lexemes[i].identifier_data.identifier->variable_name, 1, lexemes[i].identifier_data.identifier->name_length, stdout);
                printf("\n");
                break;
            case BONK_LEXEME_NUMBER:
                printf("number: %Lg\n", lexemes[i].number_data.number->float_value);
                break;
            case BONK_LEXEME_SEMICOLON:
                printf("semicolon\n");
                break;
            case BONK_LEXEME_OPERATOR:
                printf("operator: %s\n", BONK_OPERATOR_NAMES[lexemes[i].operator_data.operator_type]);
                break;
            case BONK_LEXEME_BRACE:
                printf("brace: %d\n", lexemes[i].brace_data.brace_type);
                break;
            case BONK_LEXEME_COMMA:
                printf("colon\n");
                break;
            default: break;
        }
    }
}