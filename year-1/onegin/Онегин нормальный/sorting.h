
#ifndef sorting_h
#define sorting_h

#include <stdlib.h>
#include <ctype.h>
#include "utils.h"
#include "bubblesort.h"

typedef struct my_string_view my_string_view;
typedef struct s_text s_text;
typedef enum e_walk_direction e_walk_direction;
typedef enum e_test_result e_test_result;
typedef enum e_sort_type e_sort_type;
typedef enum e_sort_result e_sort_result;

enum e_sort_type {
    E_SORT_TYPE_BUBBLESORT,
    E_SORT_TYPE_QSORT
};

enum e_walk_direction {
    E_WALK_DIRECTION_LTR,
    E_WALK_DIRECTION_RTL
};

struct my_string_view {
    const char* string;
    size_t string_length;
};

struct s_text {
    my_string_view* line_array;
    my_string_view** line_map;
    size_t line_count;
    size_t text_length;
    const char* text_buffer;
};

/**
 * @brief Compares two my_string_view structures
 * @param [in] a First structure to compare
 * @param [in] b Second structure to compare
 * @param [in] walk_direction String comparsion direction.
 * @returns Comparsion result
 */
int comparator(const void* a, const void* b, e_walk_direction walk_direction);

/**
 * @brief Compares two my_string_view structures with rtl string comparsion mode
 * @param [in] a First structure to compare
 * @param [in] b Second structure to compare
 * @returns Comparsion result
 */
int comparator_rtl(const void* a, const void* b);

/**
 * @brief Compares two my_string_view structures with ltr string comparsion mode
 * @param [in] a First structure to compare
 * @param [in] b Second structure to compare
 * @returns Comparsion result
 */
int comparator_ltr(const void* a, const void* b);

/**
 * @brief Sorts given test
 * @param [in] sort_type Specifies sorting function
 * @param [in] __compar Specifies sort comparator
 */
void sort_buffer(s_text* text, e_sort_type sort_type, int (* __compar)(const void *, const void *));


/**
 * @brief s_text constructor
 */
s_text* s_text_create(const char* buffer, size_t length);

/**
 * @brief s_text destructor
 */
void s_text_destroy(s_text* text);

/**
 * @brief Compiles s_text in specified buffer
 */
void s_text_compile(s_text* text, char* buffer);

#endif /* sorting_h */
