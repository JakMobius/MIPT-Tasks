//
// Created by Артем on 15.02.2022.
//

#include "statistics-collector.hpp"


const char* UnaryOperatorNames[] = {
        "plus",
        "minus",
        "inverse",
        "exclamation_sign",
        "construct",
        "destruct"
};

const char* BinaryOperatorNames[] = {
        "+=",
        "-=",
        "/=",
        "*=",
        "%=",
        "<<=",
        ">>=",
        "|=",
        "&=",
        "^=",
        "move",
        "copy",
        "move_assign",
        "copy_assign",
        "<",
        ">",
        "==",
        "<=",
        ">=",
};

const char* BinaryAssignmentOperatorNames[] = {
        "+",
        "-",
        "/",
        "*",
        "%",
        "<<",
        ">>",
        "|",
        "&",
        "^"
};

CollectorGuard StatisticsCollectorBase::guard(const std::string &name) {
    return { this, name };
}
