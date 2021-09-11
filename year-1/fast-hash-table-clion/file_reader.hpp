
#ifndef FAST_HASH_TABLE_CLION_FILE_READER_HPP
#define FAST_HASH_TABLE_CLION_FILE_READER_HPP

#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include "hash-table.hpp"
#include "dynamic_array.hpp"

char* read_file(const char* filepath, long long* length);
void convert_file_to_dictionary(char* dict, unsigned long long length);
char* read_dictionary_to_table(char* dict, unsigned long long length, hash_table* table, int* entries);

#endif //FAST_HASH_TABLE_CLION_FILE_READER_HPP

