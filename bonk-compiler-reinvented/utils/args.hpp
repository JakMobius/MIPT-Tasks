
#ifndef argv_parser_h
#define argv_parser_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace args {

enum arg_type {

    ARG_TYPE_FLAG,

    ARG_TYPE_KEY
};

struct flag;
struct argument;
struct argument_list;

struct flag {
    arg_type type;
    const char* flag;
    const char* description;
};

struct argument {
    const flag* flag_type;
    const char* value;
    unsigned char mentioned;
    argument* next;

    static argument* create(const flag* argument_flag);
    void construct(const flag* argument_flag);

    void destruct();
    void release();
};

struct argument_list {
    argument* initial;

    void construct(int argc, const char** argv, const flag* arguments);

    void release();
    void destruct();

    argument* search(const flag* flag_to_search);
    static argument_list* create(int argc, const char** argv, const flag* arguments);
    argument* add(const flag* flag_to_search);

    static void print_help(const flag* arguments);
};

extern const char* HELP_HEADER;
extern const flag TYPES[];

}

#endif /* argv_parser_h */
