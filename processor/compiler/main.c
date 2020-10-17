
#include "main.h"

int main(int argc, const char* argv[]) {
    
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
    
    file_op_result read_result = FILE_OP_INVALID_ARGUMENT;
    unsigned long length = 0;
    
    const char* real_path = realpath(input, NULL);
    const char* file = read_file(real_path, &read_result, &length);
    
    s_arg_view* output_file_flag = argv_view_get_arg_view(argv_view, &ARGV_TYPES[E_ARGV_NAME_OUTPUT_FILE]);
    
    const char* output_file_path;
    
    if(output_file_flag) {
        output_file_path = output_file_flag -> value;
    } else {
        output_file_path = "out.ded";
    }
    
    FILE* output_file = fopen(output_file_path, "wb");
    
    if(read_result == FILE_OP_NOT_ENOUGH_MEMORY) {
        out_of_memory_message();
        return 1;
    } else if(read_result != FILE_OP_OK || real_path == NULL) {
        printf("error: failed to read input file\n");
        return 1;
    }
    
    if(!output_file) {
        printf("error: failed to open output file\n");
        return 1;
    }
    
    bool success = compile(file, real_path, output_file);
    
    free((void*)real_path);
    free((void*)file);
    fclose(output_file);
    
    if(success) return 0;
    return 1;
}
