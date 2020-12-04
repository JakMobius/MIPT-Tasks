#include <stdio.h>
#include "tree_serializer.h"
#include "simplifier.h"
#include "grapher.h"
#include "compiler.h"

const char* get_string(){
    size_t size = 64;

    char *str = malloc(sizeof(char) * size);
    char ch;
    size_t len = 0;

    if(!str) return str;

    while(scanf("%c", &ch) == 1){
        if(ch == '\n') {
            if(len > 0) break;
            continue;
        }
        str[len++] = ch;
        if(len == size){
            size += 64;
            str = realloc(str, sizeof(char) * size);
            if(!str) return str;
        }
    }
    str[len] = '\0';

    return str;
}

void make_tex_doc(s_simplifier_config* simplifier_config, s_tree* tree) {

    FILE* output = fopen("./formula.tex", "w");
    printf("Simplifying your expression...\n");
    latex_document_begin(output);
    latex_expression_begin(output);
    latex_document_add_expression(tree, output);

    for(int i = 0; i < 1000; i++) {
        if(!tree_simplify(tree, simplifier_config)) break;
        fprintf(output, "=");
        latex_expression_break(output);
        fprintf(output, "=");
        latex_document_add_expression(tree, output);
    }

    latex_expression_end(output);
    latex_add_image("graph.png", output);
    latex_document_end(output);
    printf("Drawing graph...\n");

    bmp_draw_graph(tree, "graph.bmp");
    fclose(output);

    system("sips -s format png ./graph.bmp --out ./graph.png > /dev/null");
    printf("Creating your PDF...\n");
    system("pdflatex -synctex=1 -interaction=nonstopmode formula.tex > /dev/null");

    printf("Your PDF is ready\n");
    system("open formula.pdf");
}

s_tree* parse_tree(const char* input, s_compiler* compiler) {
    s_tree* result = parser_parse_file(compiler->parser, "<stdin>", input);
    if(!result || !result->root) return NULL;

    compiler_compile(compiler, result);

    if(compiler->state == COMPILER_OK) {
        return result;
    }

    tree_release(result);
    return NULL;
}

int main() {

    s_compiler compiler = {0};
    s_compiler_config compiler_config = {0};
    compiler_config.error_file = stderr;

    compiler_construct(&compiler, &compiler_config);
    if(compiler.state) return -1;

    s_simplifier_config simplifier_config = {0};
    simplifier_config.max_operations_per_step = 1;

    while(true) {

        const char* user_input = get_string();

        if(strcmp(user_input, "exit") == 0) {
            free((void*)user_input);
            break;
        }

        if(strcmp(user_input, "doc") == 0) {
            free((void*)user_input);

            printf("Enter your expression:\n");

            user_input = get_string();

            s_tree* expression = parse_tree(user_input, &compiler);
            if(expression) make_tex_doc(&simplifier_config, expression);
            continue;
        }

        s_tree* expression = parse_tree(user_input, &compiler);

        if(expression) {
            while(true) {
                if(!tree_simplify(expression, &simplifier_config)) break;
                tree_parser_serialize(expression, stdout);
                printf("\n");
                tree_validate(expression);
            }

            printf("Answer: ");
            tree_parser_serialize(expression, stdout);
            printf("\n");

            tree_release(expression);
        }
        free((void*)user_input);
    }

    return 0;
}
