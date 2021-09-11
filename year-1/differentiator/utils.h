//
// Created by Темыч on 07.12.2020.
//

#ifndef DIFFERENTIATOR_UTILS_H
#define DIFFERENTIATOR_UTILS_H

#include <stdio.h>
#include "tree_serializer.h"
#include "simplifier.h"
#include "grapher.h"
#include "compiler.h"
#include <time.h>

const char* get_string();
s_tree* parse_tree(const char* input, s_compiler* compiler);
struct timespec diff(struct timespec start, struct timespec end);

#endif //DIFFERENTIATOR_UTILS_H
