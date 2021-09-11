
#include "errors.h"

void compiler_warning_positioned (s_compiler* thou, s_compiler_position* pos, const char *format, ...) {
    if(thou -> config -> error_file == NULL) return;
    va_list ap;

    va_start (ap, format);
    fprintf (thou -> config -> error_file, "%s:%d:%d: warning: ", pos -> filename, pos -> line, pos -> ch);
    vfprintf (thou -> config -> error_file, format, ap);
    va_end (ap);
    fputc ('\n', thou -> config -> error_file);
}

void compiler_warning (s_compiler* thou, const char *format, ...) {
    if(thou -> config -> error_file == NULL) return;
    va_list ap;

    va_start (ap, format);
    fprintf (thou -> config -> error_file, "%s:%d:%d: warning: ", thou -> position.filename, thou -> position.line, thou -> position.ch);
    vfprintf (thou -> config -> error_file, format, ap);
    va_end (ap);
    fputc ('\n', thou -> config -> error_file);
}

void compiler_error_positioned (s_compiler* thou, s_compiler_position* pos, const char *format, ...) {
    thou -> state = COMPILER_STATE_ERROR;
    if(thou -> config -> error_file == NULL) return;
    va_list ap;

    va_start (ap, format);
    fprintf (thou -> config -> error_file, "%s:%d:%d: error: ", pos -> filename, pos -> line, pos -> ch);
    vfprintf (thou -> config -> error_file, format, ap);
    va_end (ap);
    fputc ('\n', thou -> config -> error_file);
}

void compiler_error (s_compiler* thou, const char *format, ...) {
    thou -> state = COMPILER_STATE_ERROR;
    if(thou -> config -> error_file == NULL) return;
    va_list ap;

    va_start (ap, format);
    fprintf (thou -> config -> error_file, "%s:%d:%d: error: ", thou -> position.filename, thou -> position.line, thou -> position.ch);
    vfprintf (thou -> config -> error_file, format, ap);
    va_end (ap);
    fputc ('\n', thou -> config -> error_file);
}

void compiler_fatal_error_positioned (s_compiler* thou, s_compiler_position* pos, const char *format, ...) {
    thou -> state = COMPILER_STATE_FATAL_ERROR;
    if(thou -> config -> error_file == NULL) return;
    va_list ap;

    va_start (ap, format);
    fprintf (thou -> config -> error_file, "%s:%d:%d: fatal error: ", pos -> filename, pos -> line, pos -> ch);
    vfprintf (thou -> config -> error_file, format, ap);
    va_end (ap);
    fputc ('\n', thou -> config -> error_file);
}

void compiler_fatal_error (s_compiler* thou, const char *format, ...) {
    thou -> state = COMPILER_STATE_FATAL_ERROR;
    if(thou -> config -> error_file == NULL) return;
    va_list ap;

    va_start (ap, format);
    fprintf (thou -> config -> error_file, "%s:%d:%d: fatal error: ", thou -> position.filename, thou -> position.line, thou -> position.ch);
    vfprintf (thou -> config -> error_file, format, ap);
    va_end (ap);
    fputc ('\n', thou -> config -> error_file);
}
