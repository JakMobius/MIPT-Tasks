
#include "compiler_config.h"
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
    
    bool decompile = argv_view_get_arg_view(argv_view, &ARGV_TYPES[E_ARGV_NAME_DECOMPILE]) != NULL;
    
    const char* output_file_path;
    
    if(output_file_flag) {
        output_file_path = output_file_flag -> value;
    } else {
        if(decompile) {
            output_file_path = NULL;
        } else {
            output_file_path = "out";
        }
    }
    
    FILE* output_file = NULL;
    
    if(output_file_path != NULL) {
        output_file = fopen(output_file_path, "wb");
        
        if(!output_file) {
            printf("error: failed to open output file\n");
            return 1;
        }
    }
    
    if(read_result == FILE_OP_NOT_ENOUGH_MEMORY) {
        out_of_memory_message();
        return 1;
    } else if(read_result != FILE_OP_OK || real_path == NULL) {
        printf("error: failed to read input file\n");
        return 1;
    }
    
    bool success = false;
    
    if(decompile) {
        s_decompiler_config config = {0};
        
        config.input = file;
        config.input_length = length;
        config.ignore_line_flags = argv_view_get_arg_view(argv_view, &ARGV_TYPES[E_ARGV_NAME_DECOMPILE_NO_LINE_FLAGS]) != NULL;
        if(output_file == NULL) {
            config.output_file = stdout;
        } else {
            config.output_file = output_file;
        }
        
        success = decompiler_perform(&config);
        
        if(output_file == NULL) {
            fclose(output_file);
        }
    } else {
        s_compiler_config config = {0};
        
        config.list_line_flags = false;
        
        config.insert_line_flags = argv_view_get_arg_view(argv_view, &ARGV_TYPES[E_ARGV_NAME_DEBUG_LINES]) != NULL;
        config.put_shebang = argv_view_get_arg_view(argv_view, &ARGV_TYPES[E_ARGV_NAME_NO_SHEBANG]) == NULL;
        
        config.file = file;
        config.full_path = real_path;
        config.output_file = output_file;
        config.error_file = stderr;
        
        if(argv_view_get_arg_view(argv_view, &ARGV_TYPES[E_ARGV_NAME_LISTING]) != NULL) {
            config.listing_file = stdout;
        } else {
            config.listing_file = NULL;
        }
        
        success = compiler_perform(&config);
        
        fclose(output_file);
        
        if(chmod(output_file_path, 511) < 0) {
            printf("warning: failed to add execution permissions to file\n");
        }
    }
    
    free((void*)real_path);
    free((void*)file);
    
    if(success) return 0;
    return 1;
}
