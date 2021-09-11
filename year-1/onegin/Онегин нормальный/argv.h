#ifndef argv_h
#define argv_h

#include "argv_parser.h"

typedef enum e_argv_names e_argv_names;

enum e_argv_names {
    E_ARGV_NAME_HELP,
    E_ARGV_NAME_TEST,
    E_ARGV_NAME_LTR,
    E_ARGV_NAME_RTL,
    E_ARGV_NAME_NO_SORT,
    E_ARGV_NAME_INPUT_FILE,
    E_ARGV_NAME_OUTPUT_FILE,
    E_ARGV_NAME_BUBBLESORT,
    E_ARGV_NAME_SEPARATOR
};

extern const s_flag argv_types[];

#endif /* argv_h */
