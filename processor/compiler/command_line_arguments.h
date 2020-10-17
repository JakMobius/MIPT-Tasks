#ifndef argv_h
#define argv_h

#include "../shared/argv_parser.h"

typedef enum e_argv_names e_argv_names;

enum e_argv_names {
    E_ARGV_NAME_HELP,
    E_ARGV_NAME_OUTPUT_FILE
};

const s_flag ARGV_TYPES[] = {
    {ARG_TYPE_FLAG, "h", "prints help page"},
    {ARG_TYPE_KEY,  "o", "output file (defaults to ./out.ded)"},
    {0}
};

const char* ARGV_HELP_HEADER = "Usage: compiler <input file> [flags]";

#endif /* argv_h */
