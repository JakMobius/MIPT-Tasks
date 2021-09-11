#include <stdio.h>
#include <locale.h>

#include "argv_parser.h"
#include "utils.h"
#include "argv.h"
#include "unit_tests.h"
#include "sorting.h"
#include "io.h"

void perform_unit_testing(void);
void perform_work_cycle(s_argv_view* argv_view);

/**
 * @brief Program entry pointи
 * @param [in] argc Number of command line arguments
 * @param [in] argv Command line arguments
 */
int main(int argc, const char * argv[]) {
    
    setlocale(LC_ALL, "Russian");
    
    s_argv_view* argv_view = argv_view_construct(argc, argv, argv_types);
    
    if(!argv_view) {
        OUT_OF_MEMORY_MESSAGE;
        return 0;
    }
    
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
    
    argv_view_destroy(argv_view);
    
    return 0;
}


void perform_unit_testing() {
    CALL_TEST(print_test,
            "ABC\n"
            "CAB\n"
            "BCA\n",
            "ABC\n"
            "BCA\n"
            "CAB\n",
            E_WALK_DIRECTION_LTR,
            E_SORT_TYPE_QSORT
    );
    
    CALL_TEST(print_test,
            "ABC\n"
            "CAB\n"
            "BCA\n",
            "BCA\n"
            "CAB\n"
            "ABC\n",
            E_WALK_DIRECTION_RTL,
            E_SORT_TYPE_QSORT
    );
    
    CALL_TEST(print_test,
            "ABC\n"
            "CAB\n"
            "BCA\n",
            "ABC\n"
            "BCA\n"
            "CAB\n",
            E_WALK_DIRECTION_LTR,
            E_SORT_TYPE_BUBBLESORT
    );
    
    CALL_TEST(print_test,
            "ABC\n"
            "CAB\n"
            "BCA\n",
            "BCA\n"
            "CAB\n"
            "ABC\n",
            E_WALK_DIRECTION_RTL,
            E_SORT_TYPE_BUBBLESORT
    );
    
    // ...
}

void perform_work_cycle(s_argv_view* argv_view) {
    s_arg_view* input_file_argument  = argv_view_get_arg_view(argv_view, &argv_types[E_ARGV_NAME_INPUT_FILE]);
    s_arg_view* output_file_argument = argv_view_get_arg_view(argv_view, &argv_types[E_ARGV_NAME_OUTPUT_FILE]);
    
    s_arg_view* bubblesort_flag = argv_view_get_arg_view(argv_view, &argv_types[E_ARGV_NAME_BUBBLESORT]);
    e_sort_type sort_type = bubblesort_flag == NULL ? E_SORT_TYPE_QSORT : E_SORT_TYPE_BUBBLESORT;
    
    if(input_file_argument == NULL) {
        printf("Please, specify input file\n");
        return;
    }
    
    if(output_file_argument == NULL) {
        printf("Please, specify output file\n");
        return;
    }
    
    const char* filepath = input_file_argument -> value;
    s_text* text = read_file(filepath);
    
    if(!text) {
        return;
    }
    
    FILE* output = fopen(output_file_argument -> value, "w");
    
    if(output == NULL) {
        printf("Failed to open file");
        s_text_destroy(text);
        fclose(output);
        return;
    }
    
    for(s_arg_view* arg = argv_view -> initial; arg != NULL; arg = arg -> next) {
        const s_flag* type = arg -> flag;
        
        if(type == &argv_types[E_ARGV_NAME_LTR]) {
            sort_buffer(text, sort_type, comparator_ltr);
            write_file(output, text);
        } else if(type == &argv_types[E_ARGV_NAME_RTL]) {
            sort_buffer(text, sort_type, comparator_rtl);
            write_file(output, text);
        } else if(type == &argv_types[E_ARGV_NAME_NO_SORT]) {
            fwrite(text -> text_buffer, 1, text -> text_length, output);
        }
    }
    
    fclose(output);
}
