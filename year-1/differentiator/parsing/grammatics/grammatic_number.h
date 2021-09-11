
#ifndef BONKSCRIPT_GRAMMATIC_NUMBER_H
#define BONKSCRIPT_GRAMMATIC_NUMBER_H

#include "../parser.h"

s_tree_node* parse_grammatic_number(s_parser* parser);
unsigned char char_to_digit_radix(char c, int radix);
int parse_grammatic_digits(s_parser* parser, int radix, long long* integer_value, double* float_value);

#endif //BONKSCRIPT_GRAMMATIC_NUMBER_H
