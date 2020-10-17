
#include "errors.h"

void compiler_warning_positioned (s_compiler* thou, compiler_position* pos, const char *format, ...) {
    va_list ap;

    va_start (ap, format);
    fprintf (stderr, "%s:%ld:%ld: warning: ", pos -> filename, pos -> line, pos -> ch);
    vfprintf (stderr, format, ap);
    va_end (ap);
    fputc ('\n', stderr);
}

void compiler_warning (s_compiler* thou, const char *format, ...) {
    va_list ap;

    va_start (ap, format);
    fprintf (stderr, "%s:%ld:%ld: warning: ", thou -> position.filename, thou -> position.line, thou -> position.ch);
    vfprintf (stderr, format, ap);
    va_end (ap);
    fputc ('\n', stderr);
}

void compiler_error_positioned (s_compiler* thou, compiler_position* pos, const char *format, ...) {
    thou -> state = COMPILER_STATE_ERROR;
    va_list ap;

    va_start (ap, format);
    fprintf (stderr, "%s:%ld:%ld: error: ", pos -> filename, pos -> line, pos -> ch);
    vfprintf (stderr, format, ap);
    va_end (ap);
    fputc ('\n', stderr);
}

void compiler_error (s_compiler* thou, const char *format, ...) {
    thou -> state = COMPILER_STATE_ERROR;
    va_list ap;

    va_start (ap, format);
    fprintf (stderr, "%s:%ld:%ld: error: ", thou -> position.filename, thou -> position.line, thou -> position.ch);
    vfprintf (stderr, format, ap);
    va_end (ap);
    fputc ('\n', stderr);
}
