
#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <stdlib.h>

#define OUT_OF_MEMORY_MESSAGE printf("Out of memory at %s:%d\n", __FILE__, __LINE__);

/**
 * @brief Dispays allocation error message and crashes the program afterwards
 */
void memory_failure(int, const char*);

/**
 * @returns Minimum of two size_t values
 */
size_t min(size_t a, size_t b);

/**
 * @returns Maximum of two size_t values
 */
size_t max(size_t a, size_t b);

/**
 * @brief Replaces characters in buffer
 * @param [out] buffer String that should be processed
 * @param [in] from Character to be replaced
 * @param [in] to Replacement character
 * @returns Number of replacements
 */

size_t replace_chars(char* buffer, char from, char to);

#endif /* utils_h */
