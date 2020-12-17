
#include "errors.h"
#include "parsing/lexic/bonk_lexical_analyzer.h"

void bonk_compiler_warning_positioned(s_bonk_compiler* thou, s_bonk_parser_position* pos, const char *format, ...) {
    if(thou->config->error_file == NULL) return;
    va_list ap;

    va_start (ap, format);
    fprintf (thou->config->error_file, "%s:%lu:%lu: warning: ", pos->filename, pos->line, pos->ch);
    vfprintf (thou->config->error_file, format, ap);
    va_end (ap);
    fputc ('\n', thou->config->error_file);
}

void bonk_compiler_warning(s_bonk_compiler* thou, const char *format, ...) {
    if(thou->config->error_file == NULL) return;
    va_list ap;

    va_start (ap, format);
    fprintf (thou->config->error_file, "warning: ");
    vfprintf (thou->config->error_file, format, ap);
    va_end (ap);
    fputc ('\n', thou->config->error_file);
}

void bonk_parser_warning(s_bonk_parser* thou, const char *format, ...) {
    if(thou->compiler->config->error_file == NULL) return;
    va_list ap;

    va_start (ap, format);
    s_bonk_parser_position* pos = thou->input[thou->position].position;
    fprintf (thou->compiler->config->error_file, "%s:%lu:%lu: warning: ", pos->filename, pos->line, pos->ch);
    vfprintf (thou->compiler->config->error_file, format, ap);
    va_end (ap);
    fputc ('\n', thou->compiler->config->error_file);
}

void bonk_compiler_error_positioned(s_bonk_compiler* thou, s_bonk_parser_position* pos, const char *format, ...) {
    thou->state = BONK_COMPILER_STATE_ERROR;
    if(thou->config->error_file == NULL) return;
    va_list ap;

    va_start (ap, format);
    if(pos) {
        fprintf(thou->config->error_file, "%s:%lu:%lu: error: ", pos->filename, pos->line, pos->ch);
    } else {
        fprintf(thou->config->error_file, "error: ");
    }
    vfprintf (thou->config->error_file, format, ap);
    va_end (ap);
    fputc ('\n', thou->config->error_file);
}

void bonk_compiler_error(s_bonk_compiler* thou, const char *format, ...) {
    thou->state = BONK_COMPILER_STATE_ERROR;
    if(thou->config->error_file == NULL) return;
    va_list ap;

    va_start (ap, format);
    fprintf (thou->config->error_file, "error: ");
    vfprintf (thou->config->error_file, format, ap);
    va_end (ap);
    fputc ('\n', thou->config->error_file);
}

void bonk_parser_error(s_bonk_parser* thou, const char *format, ...) {
    thou->compiler->state = BONK_COMPILER_STATE_ERROR;
    if(thou->compiler->config->error_file == NULL) return;
    va_list ap;

    va_start (ap, format);
    s_bonk_parser_position* pos = thou->input[thou->position].position;
    if(pos) {
        fprintf(thou->compiler->config->error_file, "%s:%lu:%lu: error: ", pos->filename, pos->line, pos->ch);
    } else {
        fprintf(thou->compiler->config->error_file, "error: ");
    }
    vfprintf (thou->compiler->config->error_file, format, ap);
    va_end (ap);
    fputc ('\n', thou->compiler->config->error_file);
}

void bonk_lexical_error(s_bonk_lexical_analyzer* thou, const char *format, ...) {
    thou->compiler->state = BONK_COMPILER_STATE_ERROR;
    if(thou->compiler->config->error_file == NULL) return;
    va_list ap;

    va_start (ap, format);
    s_bonk_parser_position* pos = &thou->position;
    if(pos) {
        fprintf(thou->compiler->config->error_file, "%s:%lu:%lu: error: ", pos->filename, pos->line, pos->ch);
    } else {
        fprintf(thou->compiler->config->error_file, "error: ");
    }
    vfprintf (thou->compiler->config->error_file, format, ap);
    va_end (ap);
    fputc ('\n', thou->compiler->config->error_file);
}

void bonk_compiler_fatal_error_positioned(s_bonk_compiler* thou, s_bonk_parser_position * pos, const char *format, ...) {
    thou->state = BONK_COMPILER_STATE_FATAL_ERROR;
    if(thou->config->error_file == NULL) return;
    va_list ap;

    va_start (ap, format);
    fprintf (thou->config->error_file, "%s:%lu:%lu: fatal error: ", pos->filename, pos->line, pos->ch);
    vfprintf (thou->config->error_file, format, ap);
    va_end (ap);
    fputc ('\n', thou->config->error_file);
}

void bonk_compiler_fatal_error(s_bonk_compiler* thou, const char *format, ...) {
    FILE* output = stderr;

    if(thou) {
        thou->state = BONK_COMPILER_STATE_FATAL_ERROR;
        if(thou->config->error_file == NULL) return;
        output = thou->config->error_file;
    }

    va_list ap;

    va_start (ap, format);
    fprintf (output, "fatal error: ");
    vfprintf (output, format, ap);
    va_end (ap);
    fputc ('\n', output);
}

void bonk_parser_fatal_error(s_bonk_parser* thou, const char *format, ...) {
    FILE* output = stderr;
    va_list ap;
    va_start (ap, format);

    if(thou) {
        thou->compiler->state = BONK_COMPILER_STATE_FATAL_ERROR;
        if(thou->compiler->config->error_file == NULL) return;
        output = thou->compiler->config->error_file;
        s_bonk_parser_position* pos = thou->input[thou->position].position;

        fprintf (output, "%s:%lu:%lu: fatal error: ", pos->filename, pos->line, pos->ch);
        vfprintf (output, format, ap);
    } else {
        fprintf (output, "fatal error: ");
        vfprintf (output, format, ap);
    }

    va_end (ap);
    fputc ('\n', output);
}