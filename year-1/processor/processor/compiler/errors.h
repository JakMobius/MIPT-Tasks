
#ifndef errors_h
#define errors_h

#include <stdio.h>
#include "compiler.h"

void compiler_warning_positioned(s_compiler* thou, s_compiler_position* pos, const char *format, ...);
void compiler_warning(s_compiler* thou, const char *format, ...);
void compiler_error_positioned(s_compiler* thou, s_compiler_position* pos, const char *format, ...);
void compiler_error(s_compiler* thou, const char *format, ...);
void compiler_fatal_error_positioned(s_compiler* thou, s_compiler_position* pos, const char *format, ...);
void compiler_fatal_error(s_compiler* thou, const char *format, ...);

#endif /* errors_h */
