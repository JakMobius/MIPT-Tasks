#ifndef argv_h
#define argv_h

#include "../shared/argv_parser.h"

typedef enum e_argv_names e_argv_names;

enum e_argv_names {
    E_ARGV_NAME_HELP,
    E_ARGV_NAME_OUTPUT_FILE,
    E_ARGV_NAME_DEBUG_LINES,
    E_ARGV_NAME_NO_SHEBANG,
    E_ARGV_NAME_DECOMPILE,
    E_ARGV_NAME_DECOMPILE_NO_LINE_FLAGS,
    E_ARGV_NAME_LISTING
};

const s_flag ARGV_TYPES[] = {
    {ARG_TYPE_FLAG, "h",                        "prints help page"},
    {ARG_TYPE_KEY,  "o",                        "output file (defaults to ./out.ded"},
    {ARG_TYPE_FLAG, "-debug-lines",             "enable line debugging"},
    {ARG_TYPE_FLAG, "-no-shebang",              "don't insert shebang"},
    {ARG_TYPE_FLAG, "-decompile",               "decompile existing binary file"},
    {ARG_TYPE_FLAG, "-decompile-no-line-flags", "remove 'line' instructions when decompiling"},
    {ARG_TYPE_FLAG, "-listing",                 "output listing to console"},
    {0}
};

const char* ARGV_HELP_HEADER = "Usage: compiler <input file> [flags]";

#endif /* argv_h */
