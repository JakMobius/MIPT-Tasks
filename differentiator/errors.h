
#ifndef BONKSCRIPT_ERRORS_H
#define BONKSCRIPT_ERRORS_H

#include <stdio.h>
#include <stdarg.h>
#include "./parsing/parser.h"
#include "compiler.h"

typedef struct compiler s_compiler;
typedef struct parser s_parser;
typedef struct parser_position s_parser_position;

void compiler_warning_positioned(s_compiler* thou, s_parser_position* pos, const char *format, ...);
void compiler_error_positioned(s_compiler* thou, s_parser_position* pos, const char *format, ...);
void compiler_fatal_error_positioned(s_compiler* thou, s_parser_position* pos, const char *format, ...);

void parser_warning(s_parser* thou, const char *format, ...);
void parser_error(s_parser* thou, const char *format, ...);
void parser_fatal_error(s_parser* thou, const char *format, ...);

void compiler_warning(s_compiler* thou, const char *format, ...);
void compiler_error(s_compiler* thou, const char *format, ...);
void compiler_fatal_error(s_compiler* thou, const char *format, ...);

#endif //BONKSCRIPT_ERRORS_H
