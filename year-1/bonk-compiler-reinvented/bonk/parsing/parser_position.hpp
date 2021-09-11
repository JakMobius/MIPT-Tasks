//
// Created by Темыч on 20.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_PARSER_POSITION_HPP
#define BONK_COMPILER_REINVENTED_PARSER_POSITION_HPP

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

namespace bonk {

struct parser_position {
    const char* filename;
    unsigned long line;
    unsigned long ch;
    unsigned long index;

    parser_position* clone();

    const char* to_string();
};

}

#endif //BONK_COMPILER_REINVENTED_PARSER_POSITION_HPP
