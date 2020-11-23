//
// Created by Темыч on 15.11.2020.
//

#ifndef AKINATOR_AKINATOR_H
#define AKINATOR_AKINATOR_H

#include "tree.h"
#include "tree_parser.h"
#include <stdbool.h>
#include <zconf.h>
#include <string.h>
#include <errno.h>
#include "tree_searcher.h"

const char* akinator_ask_string();
bool akinator_ask(const char* format, ...);

void main_akinator_loop(s_tree* tree);
void akinator_describe(s_tree* tree);
void akinator_guess(s_tree* tree);
void akinator_difference(s_tree* tree);
void akinator_help();
void akinator_dump(s_tree* tree);

void akinator_ask_for_object(s_tree* tree, const bool** path_buffer, const char** name_buffer);

#endif //AKINATOR_AKINATOR_H
