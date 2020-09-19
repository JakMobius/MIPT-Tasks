
#ifndef argv_parser_h
#define argv_parser_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

typedef enum e_arg_type {
    /**
     * @brief This type means that the flag does not store any additional values
     */
    ARG_TYPE_FLAG,
    
    /**
     * @brief This type means that the flag stores additional string in next command line arguments
     */
    ARG_TYPE_KEY
} e_arg_type;

typedef struct s_flag s_flag;
typedef struct s_arg_view s_arg_view;
typedef struct s_argv_view s_argv_view;


/**
 * @brief command line frgument descriptor
 */
struct s_flag {
    /**
     * @brief flag name without first dash
     * @discussion to create "--flag" command line flag, set this field to "-flag"
     */
    const char* flag;
    
    /**
     * @brief flag description. This field is used when printing help page
     */
    const char* description;
    
    /**
     * @brief flag type
     */
    e_arg_type type;
};

/**
 * @brief parsed argument container. Stored in s_argv_view for each met flag
 */
struct s_arg_view {
    /**
     * @brief flag descriptor
     */
    const s_flag* flag;
    
    /**
     * @brief flag value if available
     */
    const char* value;
    
    /**
     * @brief this field indicates how many times this flag met
     */
    unsigned char mentioned;
    s_arg_view* next;
};

struct s_argv_view {
    s_arg_view* initial;
};

/**
 * @brief s_argv_view constructor
 * @param [in] argc Number of command line arguments to be parsed
 * @param [in] argv Command line arguments to be parsed
 * @param [in] arguments Argument descriptor array. Should end with zeroed entry
 * @returns new s_argv_view instance
 */

s_argv_view* argv_view_construct(int argc, const char** argv, const s_flag* arguments);

/**
 * @brief s_argv_view destructor
 * @param argv_view s_argv_view to be destructed
 */

void argv_view_destroy(s_argv_view* argv_view);

/**
 * @brief reads specified flag from s_argv_view
 * @param [in] argv_view s_argv_view instance
 * @param [in] argument Argument descriptor to search
 */
s_arg_view* argv_view_get_arg_view(const s_argv_view* argv_view, const s_flag* argument);
void argv_print_help(const s_flag* arguments);

#endif /* argv_parser_h */
