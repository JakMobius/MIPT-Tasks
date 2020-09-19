
#include "argv.h"

const s_flag argv_types[] = {
    {"h", "prints help page", ARG_TYPE_FLAG},
    {"-test", "perform unit testing", ARG_TYPE_FLAG},
    {"-ltr", "specifies sorting order left to right", ARG_TYPE_FLAG},
    {"-rtl", "specifies sorting order right to left", ARG_TYPE_FLAG},
    {"i", "input file", ARG_TYPE_KEY},
    {"o", "output file", ARG_TYPE_KEY},
    {NULL, NULL, 0}
};
