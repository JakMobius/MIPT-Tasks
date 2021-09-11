//
// Created by Темыч on 07.12.2020.
//

#include "command_line_arguments.h"

const s_flag ARGV_TYPES[] = {
        {ARG_TYPE_FLAG, "h", "prints help page"},
        {ARG_TYPE_KEY,  "o", "output file (defaults to ./out.ded"},
        {0}
};

const char* ARGV_HELP_HEADER = "Usage: bs <input file> [flags]";