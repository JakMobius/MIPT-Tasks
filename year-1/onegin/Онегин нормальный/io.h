#ifndef io_h
#define io_h

#include <stdlib.h>
#include <sys/stat.h>
#include "utils.h"
#include "sorting.h"

/**
 * @brief Reads file at given path
 * @param [in] filepath Path of file to read
 * @returns Buffer with file contents
 */
s_text* read_file(const char* filepath);

/**
 * @brief Writes file at given path
 * @param [in] file File to write
 * @param [in] text Text to write
 */
void write_file(FILE* file, s_text* text);


#endif /* io_h */
