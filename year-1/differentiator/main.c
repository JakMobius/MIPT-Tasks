#include <stdio.h>
#include "tree_serializer.h"
#include "simplifier.h"
#include "grapher.h"
#include "compiler.h"
#include "utils.h"
#include "phrases.h"
#include <time.h>

void make_tex_doc(s_simplifier_config* simplifier_config, s_tree* expression);
void simplify(s_simplifier_config* simplifier_config, s_tree* expression);

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
            tree_release(expression);
        } else {
            s_tree* expression = parse_tree(user_input, &compiler);
            if(expression) simplify(&simplifier_config, expression);
            tree_release(expression);
        }

//        printf("%d nodes in memory after cleanup\n", tree_node_amount);

        free((void*)user_input);
    }

    return 0;
}

void make_tex_doc(s_simplifier_config* simplifier_config, s_tree* expression) {

    FILE* output = fopen("./formula.tex", "w");
    printf("Simplifying your expression...\n");
    latex_document_begin(output);

    srand(time(0));

    fprintf(output, "%s", NONSENSE_HEADER);

    latex_expression_begin(output);
    latex_document_add_expression(expression, output, NULL);
    latex_expression_end(output);

    s_simplifier_context ctx = {};
    simplifier_context_init(&ctx);

    for(volatile int i = 0;;i++) {
        if(!tree_simplify(&ctx, expression, simplifier_config)) break;

        if(ctx.taken_derivative) {
            fprintf(output, "%s", NONSENSE_DERIVATIVE_PHRASES[rand() % NONSENSE_DERIVATIVE_PHRASES_AMOUNT]);
        } else {
            fprintf(output, "%s", NONSENSE_PHRASES[rand() % NONSENSE_PHRASES_AMOUNT]);
        }

        latex_expression_begin(output);
        latex_document_add_expression(expression, output, ctx.after_simplification);
        latex_expression_end(output);
    }

    simplifier_context_destruct(&ctx);

    fprintf(output, "%s", NONSENSE_GRAPH_HEADER);

    latex_add_image("graph.png", output);

    fprintf(output, "%s", NONSENSE_FOOTER);

    latex_document_end(output);
    printf("Drawing graph...\n");

    bmp_draw_graph(expression, "graph.bmp");
    fclose(output);

    system("sips -s format png ./graph.bmp --out ./graph.png > /dev/null");
    printf("Creating your PDF...\n");
    system("pdflatex -synctex=1 -interaction=nonstopmode formula.tex > /dev/null");

    printf("Your PDF is ready\n");
    system("open formula.pdf");
}

void simplify(s_simplifier_config* simplifier_config, s_tree* expression) {
    struct timespec time1, time2;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

    s_simplifier_context ctx = {0};
    simplifier_context_init(&ctx);

    while(true) {
        if(!tree_simplify(&ctx, expression, simplifier_config)) break;
//        printf("%d tree nodes are now stored in memory\n", tree_node_amount);
        tree_validate(expression);
        tree_parser_serialize(expression, stdout);
        printf("\n\n");
    }

    simplifier_context_destruct(&ctx);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

    printf("Answer: ");
    tree_parser_serialize(expression, stdout);
    printf("\n");
//    printf("Simplifying took %ld.%09lds\n", diff(time1, time2).tv_sec, diff(time1, time2).tv_nsec);

//    printf("%d nodes in memory\n", tree_node_amount);
}