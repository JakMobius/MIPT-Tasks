
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
 * @param [in] text Text to be written
 * @param [out] buffer Target buffer
 */

void s_text_compile(s_text* text, char* buffer) {
    size_t string_index = 0;

    for(size_t i = 0; i < text -> line_count; i++) {
        const char* string = text -> line_map[i] -> string;
        size_t length = text -> line_map[i] -> string_length;
        size_t index = 0;
        
        while(index < length) {
            buffer[string_index++] = string[index++];
        }
        
        buffer[string_index++] = '\n';
    }
    
    buffer[string_index] = '\0';
}

void sort_buffer(s_text* text, e_sort_type sort_type, int (* comparator)(const void *, const void *)) {
    
    void (*sort_function)(void *__base, size_t __nel, size_t __width, int (* comparator)(const void *, const void *));
    
    if(sort_type == E_SORT_TYPE_QSORT) {
        sort_function = &qsort;
    } else {
        sort_function = &bubble_sort;
    }
    
    (*sort_function)(text -> line_map, text -> line_count, sizeof(my_string_view**), comparator);
}

s_text* s_text_create(const char* buffer, size_t length) {
    
    size_t line_count = strchrcnt(buffer, '\n', length);
    
    my_string_view* line_array = (my_string_view*) malloc(line_count * sizeof(my_string_view));
    my_string_view** line_map = (my_string_view**) malloc(line_count * sizeof(my_string_view*));
    
    if(!line_map || !line_array) {
        free(line_map);
        free(line_array);
        
        OUT_OF_MEMORY_MESSAGE;
        return NULL;
    }
    
    size_t line_start = 0;
    size_t line_index = 0;

    for(size_t i = 0; i < length; i++) {
        if(buffer[i] == '\n') {
            line_array[line_index].string = buffer + line_start;
            line_array[line_index].string_length = i - line_start;
            
            line_map[line_index] = &line_array[line_index];
            line_start = i + 1;
            
            line_index++;
        }
    }
    
    s_text* text = (s_text*)malloc(sizeof(s_text));
    
    text -> text_buffer = buffer;
    text -> line_array = line_array;
    text -> line_map = line_map;
    text -> line_count = line_index;
    text -> text_length = length;
    
    return text;
}

void s_text_destroy(s_text* text) {
    free(text -> line_array);
    free(text -> line_map);
    free(text);
}
