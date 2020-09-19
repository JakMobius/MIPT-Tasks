#ifndef io_h
#define io_h

#include <stdlib.h>
#include <sys/stat.h>
#include "utils.h"

/**
 * @brief Reads file at given path
 * @param [in] filepath Path of file to read
 * @param [out] length File length
 * @returns Buffer with file contents
 */
char* read_file(const char* filepath, long long* length);

/**
 * @brief Writes file at given path
 * @param [in] filepath Path of file to write
 * @param [in] buffer String to write
 */
void write_file(const char* filepath, char* buffer);


#endif /* io_h */
