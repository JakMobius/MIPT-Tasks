
#ifndef sorting_h
#define sorting_h

#include <stdlib.h>
#include <ctype.h>
#include "utils.h"

typedef struct my_string_view my_string_view;
typedef enum e_walk_direction e_walk_direction;
typedef enum e_test_result e_test_result;

enum e_walk_direction {
    E_WALK_DIRECTION_LTR,
    E_WALK_DIRECTION_RTL
};

struct my_string_view {
    const char* string;
    size_t string_length;
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

void sort_buffer(const char* buffer, size_t line_count, size_t buffer_length, char* out_buffer, e_walk_direction mode);

#endif /* sorting_h */
