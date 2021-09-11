
#include "argv_parser.h"

s_arg_view* arg_view_construct(const s_flag* argument) {
    s_arg_view* arg_view = (s_arg_view*)malloc(sizeof(s_arg_view));
    if(!arg_view) {
        return NULL;
    }
    
    arg_view -> flag = argument;
    arg_view -> value = NULL;
    arg_view -> next = NULL;
    arg_view -> mentioned = 0;
    
    return arg_view;
}

void arg_view_destroy(s_arg_view* argument) {
    if(argument -> next != NULL) {
        arg_view_destroy(argument -> next);
    }
    
    free(argument);
}

const s_flag* find_argument(const s_flag* arguments, const char* flag) {
    const s_flag* argument = arguments;
    while(argument -> flag) {
        if(strcmp(argument -> flag, flag) == 0) {
            return argument;
        }
        
        argument++;
    }
    
    return NULL;
}

s_arg_view* argv_view_use_arg_view(s_argv_view* argv_view, const s_flag* argument) {
    s_arg_view* arg_view = argv_view -> initial;
    
    if(arg_view == NULL) {
        argv_view -> initial = arg_view_construct(argument);

        if(!argv_view -> initial) {
            return NULL;
        }
        
        return argv_view -> initial;
    }
    
    while(1) {
        if(arg_view -> flag != argument) {
            if(arg_view -> next == NULL) {
                arg_view -> next = arg_view_construct(argument);
                return arg_view -> next;
            } else {
                arg_view = arg_view -> next;
            }
        } else {
            return arg_view;
        }
    }
}

s_argv_view* argv_view_construct(int argc, const char** argv, const s_flag* arguments) {
    s_argv_view* argv_view = (s_argv_view*)malloc(sizeof(s_argv_view));
    if(!argv_view) {
        return NULL;
    }
    
    const s_flag* current_flag = NULL;
    
    argv_view -> initial = NULL;
    
    for(int i = 0; i < argc; i++) {
        const char* arg = argv[i];
        
        if(current_flag) {
            s_arg_view* arg_view = argv_view_use_arg_view(argv_view, current_flag);
            if(!arg_view) {
                argv_view_destroy(argv_view);
                return NULL;
            }
            
            arg_view -> mentioned++;
            arg_view -> value = arg;
            current_flag = NULL;
            continue;
        }
        
        if(*arg == '\0') continue;
        if(*arg == '-') {
            const s_flag* argument = find_argument(arguments, arg + 1);
            
            if(argument == NULL) {
                printf("Unknown argument: %s\n", arg);
                continue;
            }
            
            if(argument -> type == ARG_TYPE_KEY) {
                current_flag = argument;
            } else {
                s_arg_view* arg_view = argv_view_use_arg_view(argv_view, argument);
                if(!arg_view) {
                    argv_view_destroy(argv_view);
                    return NULL;
                }
                
                arg_view -> mentioned++;
            }
        }
    }
    
    if(current_flag != NULL) {
        printf("Flag '%s' requires value\n", current_flag -> flag);
    }
    
    return argv_view;
}

void argv_view_destroy(s_argv_view* argv_view) {
    if(argv_view -> initial)
        arg_view_destroy(argv_view -> initial);
    free(argv_view);
}

s_arg_view* argv_view_get_arg_view(const s_argv_view* argv_view, const s_flag* argument) {
    s_arg_view* arg_view = argv_view -> initial;
    
    if(arg_view == NULL) return NULL;
    
    while(1) {
        if(arg_view -> flag != argument) {
            if(arg_view -> next == NULL) {
                return NULL;
            } else {
                arg_view = arg_view -> next;
            }
        } else {
            return arg_view;
        }
    }
}

void argv_print_help(const s_flag* arguments) {
    printf("%s\nCommand line flags:\n", ARGV_HELP_HEADER);
    
    int max_flag_length = 0;
    
    const s_flag* argument = arguments;
    while(argument -> flag) {
        
        int flag_length = (int)strlen(argument -> flag);
        
        if(flag_length > max_flag_length) {
            max_flag_length = flag_length;
        }
        
        argument++;
    }
    
    argument = arguments;
    
    while(argument -> flag) {
        
        int spacing = (int)(max_flag_length - strlen(argument -> flag) + 2);
        
        printf(" -%s%*c%s\n", argument -> flag, spacing, ' ', argument -> description);
        
        argument++;
    }
}
