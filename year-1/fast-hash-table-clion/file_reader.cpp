
#include "file_reader.hpp"

char* read_file(const char* filepath, long long* length) {
    struct stat filedata = {};
    if(stat(filepath, &filedata) == -1) {
        return nullptr;
    }

    *length = filedata.st_size;

    char* buffer = (char*) calloc(1, *length);

    if(!buffer) {
        return nullptr;
    }

    FILE* input = fopen(filepath, "rb");
    fread(buffer, 1, *length, input);
    fclose(input);

    return buffer;
}

void convert_file_to_dictionary(char* dict, unsigned long long length) {
    for(int i = 0; i < length; i++) {
        if(dict[i] == '=' || dict[i] == '\n' || dict[i] == '\r') dict[i] = 0;
    }
}

char* read_dictionary_to_table(char* dict, unsigned long long length, hash_table* table, int* entries) {

    s_dynamic_array<char*> keys = {};
    s_dynamic_array<char*> values = {};
    keys.construct(256);
    values.construct(256);

    char* key = dict;
    char* value = nullptr;

    for(int i = 0; i < length; i++) {
        if(dict[i] == '\0') {
            while(dict[i] == '\0' && i < length) i++;
            if(i == length) break;

            if(!value) {
                value = dict + i;
            } else {
                keys.push(key);
                values.push(value);
                key = dict + i;
                value = nullptr;
            }
        }
    }

    if(value) {
        keys.push(key);
        values.push(value);
    }

    if(entries) *entries = keys.size;
    char* keys_for_table = (char*)calloc(keys.size, sizeof(char) * 64);

    for(int i = 0; i < keys.size; i++) {
        char* next_key = keys.get(i);
        char* table_key = keys_for_table + i * 64;
        for(int j = 0; j < 64 && next_key[j]; j++) {
            table_key[j] = next_key[j];
        }

        table->insert(table_key, values.get(i), nullptr);
    }

    values.destruct();
    keys.destruct();

    return keys_for_table;
}