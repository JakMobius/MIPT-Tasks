
#include "args.hpp"

namespace args {

argument* argument::create(const flag* argument_flag) {
    auto* new_argument = (argument*)calloc(1, sizeof(argument));
    new_argument->construct(argument_flag);
    return new_argument;
}

void argument::construct(const flag* argument_flag) {
    flag_type = argument_flag;
    value = nullptr;
    next = nullptr;
    mentioned = 0;
}

void argument::destruct() {
    if(next != nullptr) next->release();
}

void argument::release() {
    destruct();
    free(this);
}

const flag* find_argument(const flag* arguments, const char* string) {
    const flag* argument = arguments;
    while(argument->flag) {
        if(strcmp(argument->flag, string) == 0) {
            return argument;
        }

        argument++;
    }

    return nullptr;
}

argument* argument_list::add(const flag* flag_to_search) {
    argument* current_argument = initial;

    if(current_argument == nullptr) {
        initial = argument::create(flag_to_search);

        if(!initial) {
            return nullptr;
        }

        return initial;
    }

    while(true) {
        if(current_argument->flag_type != flag_to_search) {
            if(current_argument->next == nullptr) {
                current_argument->next = argument::create(flag_to_search);
                return current_argument->next;
            } else {
                current_argument = current_argument->next;
            }
        } else {
            return current_argument;
        }
    }
}

argument_list* argument_list::create(int argc, const char** argv, const flag* arguments) {
    auto* argv_view = (argument_list*)calloc(1, sizeof(argument_list));
    if(!argv_view) return nullptr;
    argv_view->construct(argc, argv, arguments);
    return argv_view;
}

void argument_list::construct(int argc, const char** argv, const flag* arguments) {
    const flag* current_flag = nullptr;

    initial = nullptr;

    for(int i = 0; i < argc; i++) {
        const char* arg = argv[i];

        if(current_flag) {
            argument* arg_view = add(current_flag);
            if(!arg_view) {
                destruct();
                return;
            }

            arg_view->mentioned++;
            arg_view->value = arg;
            current_flag = nullptr;
            continue;
        }

        if(*arg == '\0') continue;
        if(*arg == '-') {
            const flag* found_flag = find_argument(arguments, arg + 1);

            if(found_flag == nullptr) {
                printf("Unknown argument: %s\n", arg);
                continue;
            }

            if(found_flag->type == ARG_TYPE_KEY) {
                current_flag = found_flag;
            } else {
                argument* new_argument = add(found_flag);
                if(!new_argument) {
                    destruct();
                    return;
                }

                new_argument->mentioned++;
            }
        }
    }

    if(current_flag != nullptr) {
        printf("Flag '%s' requires value\n", current_flag->flag);
    }
}

void argument_list::destruct() {
    if(initial) {
        initial->destruct();
        initial = nullptr;
    }
}

void argument_list::release() {
    destruct();
    free(this);
}

argument* argument_list::search(const flag* flag_to_search) {
    argument* arg_view = initial;

    if(arg_view == nullptr) return nullptr;

    while(true) {
        if(arg_view->flag_type == flag_to_search) {
            return arg_view;
        } else {
            if(arg_view->next == nullptr) {
                return nullptr;
            } else {
                arg_view = arg_view->next;
            }
        }
    }
}

void argument_list::print_help(const flag* arguments) {
    printf("%s\nCommand line flags:\n", HELP_HEADER);

    int max_flag_length = 0;

    const flag* argument = arguments;
    while(argument->flag) {

        int flag_length = (int)strlen(argument->flag);

        if(flag_length > max_flag_length) {
            max_flag_length = flag_length;
        }

        argument++;
    }

    argument = arguments;

    while(argument->flag) {

        int spacing = (int)(max_flag_length - strlen(argument->flag) + 2);

        printf(" -%s%*c%s\n", argument->flag, spacing, ' ', argument->description);

        argument++;
    }
}

}
