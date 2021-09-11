//
// Created by Темыч on 21.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_JSON_SERIALIZER_HPP
#define BONK_COMPILER_REINVENTED_JSON_SERIALIZER_HPP

#include <cstdio>
#include "dynamic_array.cpp"
#include "default_alloc_construct.hpp"

struct json_serializer_state {
    bool is_array;
    bool is_first;
};

struct json_serializer {
    FILE* target;
    int depth;
    json_serializer_state state;
    dynamic_array<json_serializer_state> states;

    static json_serializer* create(FILE* file);
    void construct(FILE* file);
    void block_string_field(const char* name, const char* value);
    void block_number_field(const char* name, long double value);
    void block_start_block(const char* name);
    void block_start_array(const char* name);
    void close_block();

    void array_add_string(const char* name);
    void array_add_number(long double name);
    void array_add_block();
    void array_add_array();
    void close_array();
    void padding();
    void escape_string(const char* string);

    void prepare_next_field();

    void release();

    void destroy();
};

#endif //BONK_COMPILER_REINVENTED_JSON_SERIALIZER_HPP
