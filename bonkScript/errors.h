
#ifndef BONKSCRIPT_ERRORS_H
#define BONKSCRIPT_ERRORS_H

#include <stdio.h>
#include <stdarg.h>
#include "./parsing/bonk_parser.h"
#include "bonk_compiler.h"

typedef struct bonk_compiler s_bonk_compiler;
typedef struct bonk_parser s_bonk_parser;
typedef struct bonk_parser_position s_bonk_parser_position;
typedef struct bonk_lexical_analyzer s_bonk_lexical_analyzer;

void bonk_compiler_warning_positioned(s_bonk_compiler* thou, s_bonk_parser_position* pos, const char *format, ...);
void bonk_compiler_error_positioned(s_bonk_compiler* thou, s_bonk_parser_position* pos, const char *format, ...);
void bonk_compiler_fatal_error_positioned(s_bonk_compiler* thou, s_bonk_parser_position* pos, const char *format, ...);

void bonk_parser_warning(s_bonk_parser* thou, const char *format, ...);
void bonk_parser_error(s_bonk_parser* thou, const char *format, ...);
void bonk_parser_fatal_error(s_bonk_parser* thou, const char *format, ...);
void bonk_lexical_error(s_bonk_lexical_analyzer* thou, const char *format, ...);

void bonk_compiler_warning(s_bonk_compiler* thou, const char *format, ...);
void bonk_compiler_error(s_bonk_compiler* thou, const char *format, ...);
void bonk_compiler_fatal_error(s_bonk_compiler* thou, const char *format, ...);

#endif //BONKSCRIPT_ERRORS_H
