
#ifndef BONKSCRIPT_BONK_NUMBER_LEXEME_H
#define BONKSCRIPT_BONK_NUMBER_LEXEME_H

#include "./bonk_lexical_analyzer.h"

bool bonk_parse_number_lexeme(s_bonk_lexical_analyzer* analyzer, s_bonk_lexeme* target);
unsigned char char_to_digit_radix(char c, int radix);
int bonk_parser_digits_lexeme(s_bonk_lexical_analyzer* analyzer, int radix, long long* integer_value, double* float_value);

#endif //BONKSCRIPT_BONK_NUMBER_LEXEME_H
