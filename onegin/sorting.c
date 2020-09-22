
#include "sorting.h"

/**
 * @brief Compares two my_string_view structures
 * @param [in] a First structure to compare
 * @param [in] b Second structure to compare
 * @param [in] walk_direction String comparsion direction.
 * @returns Comparsion result
 */
int comparator(const void* a, const void* b, e_walk_direction walk_direction) {
    my_string_view* first = *(my_string_view**) a;
    my_string_view* second = *(my_string_view**) b;
    
    const char* first_string;
    const char* second_string;
    
    const char* first_string_end;
    const char* second_string_end;
    
    int delta;
    
    if(walk_direction == E_WALK_DIRECTION_RTL) {
        first_string = first -> string + first -> string_length - 1;
        second_string = second -> string + second -> string_length - 1;
        
        first_string_end = first -> string - 1;
        second_string_end = second -> string - 1;
        
        delta = -1;
    } else {
        first_string = first -> string;
        second_string = second -> string;
        
        first_string_end = first_string + first -> string_length;
        second_string_end = second_string + second -> string_length;
        
        delta = 1;
    }
    
    char char_a = '\0';
    char char_b = '\0';
    
    while(first_string != first_string_end && second_string != second_string_end) {
        
        char_a = *first_string;
        char_b = *second_string;
        
        if(isspace(char_a) || ispunct(char_a)) {
            first_string += delta;
            continue;
        }

        if(isspace(char_b) || ispunct(char_b)) {
            second_string += delta;
            continue;
        }
        
        if(char_a > char_b) {
            return 1;
        }
        if(char_a < char_b) {
            return -1;
        }
        
        first_string += delta;
        second_string += delta;
    }
    
    return 0;
}

int comparator_rtl(const void* a, const void* b) {
    return comparator(a, b, E_WALK_DIRECTION_RTL);
}

int comparator_ltr(const void* a, const void* b) {
    return comparator(a, b, E_WALK_DIRECTION_LTR);
}

/**
 * @brief Writes my_string_view array to buffer
 * @param [in] lines Lines to be written
 * @param [in] line_count Number of lines
 * @param [out] buffer Target file
 */

void compile_lines(const my_string_view** lines, size_t line_count, char* buffer) {
    size_t string_index = 0;
    
    char c = '\0';
    
    for(size_t i = 0; i < line_count; i++) {
        const char* string = lines[i] -> string;
        while((c = *string++) != '\0') {
            buffer[string_index++] = c;
        }
        
        buffer[string_index++] = '\n';
    }
    
    buffer[string_index] = '\0';
}

e_sort_result sort_buffer(const char* buffer, size_t line_count, size_t buffer_length, char* out_buffer, e_walk_direction walk_direction, e_sort_type sort_type) {
    my_string_view* line_array = (my_string_view*) malloc(line_count * sizeof(my_string_view));
    const my_string_view** line_map = (const my_string_view**) malloc(line_count * sizeof(my_string_view*));
    
    if(!line_map || !line_array || !out_buffer) {
        free(line_map);
        free(line_array);
        free(out_buffer);
        
        OUT_OF_MEMORY_MESSAGE;
        return E_SORT_RESULT_OUT_OF_MEMORY;
    }
    
    size_t line_start = 0;
    size_t line_index = 0;

    for(size_t i = 0; i < buffer_length; i++) {
        if(buffer[i] == '\0') {
            line_array[line_index].string = buffer + line_start;
            line_array[line_index].string_length = i - line_start;
            
            line_map[line_index] = &line_array[line_index];
            line_start = i + 1;
            
            line_index++;
        }
    }
    
    void (*sort_function)(void *__base, size_t __nel, size_t __width, int (* __compar)(const void *, const void *));
    
    if(sort_type == E_SORT_TYPE_QSORT) {
        sort_function = &qsort;
    } else {
        sort_function = &bubble_sort;
    }
    
    if(walk_direction == E_WALK_DIRECTION_LTR) {
        (*sort_function)(line_map, line_count, sizeof(*line_map), &comparator_ltr);
    } else {
        (*sort_function)(line_map, line_count, sizeof(*line_map), &comparator_rtl);
    }
    
    compile_lines(line_map, line_count, out_buffer);
    
    free(line_array);
    free(line_map);
    
    return E_SORT_RESULT_SUCCESS;
}
