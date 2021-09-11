
#ifndef BONKSCRIPT_BONK_IDENTIFIER_LEXEME_H
#define BONKSCRIPT_BONK_IDENTIFIER_LEXEME_H

#include "./bonk_lexical_analyzer.h"

bool bonk_parse_identifier_lexeme(s_bonk_lexical_analyzer* analyzer, s_bonk_lexeme* target);
e_bonk_keyword_type bonk_keyword_from_string(const char* string, unsigned long length);

#endif //BONKSCRIPT_BONK_IDENTIFIER_LEXEME_H
