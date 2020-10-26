
#include "main.h"

int main(int argc, const char * argv[]) {
    
    if(argc <= 1) {
        argv_print_help(ARGV_TYPES);
        return 0;
    }
    
    const char* input = argv[1];
    
    s_argv_view* argv_view = argv_view_construct(argc - 2, argv + 2, ARGV_TYPES);
    
    if(!argv_view) {
        out_of_memory_message();
        return 0;
    }
    
    s_arg_view* help_flag = argv_view_get_arg_view(argv_view, &ARGV_TYPES[E_ARGV_NAME_HELP]);
    
    if(help_flag != NULL || input[0] == '-') {
        argv_print_help(ARGV_TYPES);
        return 0;
    }
    
    unsigned long length = 0;
    file_op_result read_result = FILE_OP_INVALID_ARGUMENT;
    const char* file = read_file(input, &read_result, &length);
    
    if(read_result == FILE_OP_READ_ERROR) {
        perror("Failed to read file");
        return 0;
    } else if(read_result == FILE_OP_NOT_ENOUGH_MEMORY) {
        out_of_memory_message();
        return 0;
    } else if(read_result != FILE_OP_OK) {
        printf("Failed to read file");
        return 0;
    }
    
    proc_interpret(file, length);
    
    return 0;
}
