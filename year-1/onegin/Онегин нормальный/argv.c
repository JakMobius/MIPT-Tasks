
#include "argv.h"

const s_flag argv_types[] = {
    {ARG_TYPE_FLAG, "h",           "prints help page"},
    {ARG_TYPE_FLAG, "-test",       "perform unit testing"},
    {ARG_TYPE_FLAG, "-ltr",        "specifies sorting order left to right"},
    {ARG_TYPE_FLAG, "-rtl",        "specifies sorting order right to left"},
    {ARG_TYPE_FLAG, "-no-sort",    "output original file"},
    {ARG_TYPE_KEY,  "i",           "input file"},
    {ARG_TYPE_KEY,  "o",           "output file"},
    {ARG_TYPE_FLAG, "-bubblesort", "uses bubble sort instead of qsort"},
    {ARG_TYPE_KEY,  "-separator",  "specifies separator between sorted sections"},
    {0}
};
