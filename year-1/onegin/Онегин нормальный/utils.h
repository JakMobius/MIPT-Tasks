
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
 * @brief Counts characters in buffer
 * @param [out] buffer String that should be processed
 * @param [in] c Char to search
 * @returns Number of replacements
 */

size_t strchrcnt(const char* buffer, char c, size_t);

#endif /* utils_h */
