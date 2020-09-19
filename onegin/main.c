#include <stdio.h>
#include <locale.h>

#include "argv_parser.h"
#include "utils.h"
#include "argv.h"
#include "unit_tests.h"
#include "sorting.h"
#include "io.h"

void perform_unit_testing() {
    CALL_TEST(print_test,
            "ABC\n"
            "CAB\n"
            "BCA\n",
            "ABC\n"
            "BCA\n"
            "CAB\n",
            E_WALK_DIRECTION_LTR
    );
    
    CALL_TEST(print_test,
            "ABC\n"
            "CAB\n"
            "BCA\n",
            "BCA\n"
            "CAB\n"
            "ABC\n",
            E_WALK_DIRECTION_RTL
    );
    
    // ...
}

void perform_work_cycle(s_argv_view* argv_view) {
    s_arg_view* input_file_argument = argv_view_get_arg_view(argv_view, &argv_types[E_ARGV_NAME_INPUT_FILE]);
    s_arg_view* output_file_argument = argv_view_get_arg_view(argv_view, &argv_types[E_ARGV_NAME_OUTPUT_FILE]);
    
    s_arg_view* ltr_flag = argv_view_get_arg_view(argv_view, &argv_types[E_ARGV_NAME_LTR]);
    s_arg_view* rtl_flag = argv_view_get_arg_view(argv_view, &argv_types[E_ARGV_NAME_RTL]);
    
    if(input_file_argument == NULL) {
        printf("Please, specify input file\n");
        return;
    }
    
    if(output_file_argument == NULL) {
        printf("Please, specify output file\n");
        return;
    }
    
    if(ltr_flag != NULL && rtl_flag != NULL) {
        printf("Both --rtl and --ltr flags specified. Falling back to --ltr\n");
    }
    
    if(ltr_flag == NULL && rtl_flag == NULL) {
        printf("Neither --rtl or --ltr flags specified. Using --ltr by default\n");
    }
    
    long long file_size = 0;
    
    const char* filepath = input_file_argument -> value;
    char* buffer = read_file(filepath, &file_size);
    
    if(!buffer) {
        perror("Failed to read file");
        return;
    }
    
    char ltr = !rtl_flag || ltr_flag;
    
    size_t line_num = replace_chars(buffer, '\n', '\0');
    char* out_buffer = (char*)malloc(file_size * sizeof(char));
    sort_buffer(buffer, line_num, file_size, out_buffer, ltr);
    write_file(output_file_argument -> value, out_buffer);
    
    free(buffer);
    free(out_buffer);
    
    argv_view_destroy(argv_view);
}

/**
 * @brief Program entry point
 * @param [in] argc Number of command line arguments
 * @param [in] argv Command line arguments
 */
int main(int argc, const char * argv[]) {
    
    setlocale(LC_ALL, "Russian");
    
    s_argv_view* argv_view = argv_view_construct(argc, argv, argv_types);
    
    s_arg_view* help_flag = argv_view_get_arg_view(argv_view, &argv_types[E_ARGV_NAME_HELP]);
    
    if(help_flag != NULL) {
        argv_print_help(argv_types);
        return 0;
    }
    
    s_arg_view* test_flag = argv_view_get_arg_view(argv_view, &argv_types[E_ARGV_NAME_TEST]);
    
    if(test_flag != NULL) {
        perform_unit_testing();
    } else {
        perform_work_cycle(argv_view);
    }
    
    return 0;
}
