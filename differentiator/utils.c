
#include "utils.h"

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

struct timespec diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec-start.tv_sec - 1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}