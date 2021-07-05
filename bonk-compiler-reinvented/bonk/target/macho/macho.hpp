//
// Created by Темыч on 03.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_MACHO_HPP
#define BONK_COMPILER_REINVENTED_MACHO_HPP

#include <mach/machine.h>
#include <mach/mach.h>
#include <mach-o/reloc.h>
#include <mach-o/nlist.h>
#include <mach-o/loader.h>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include "../../../utils/default_alloc_construct.hpp"
#include "../../../utils/dynamic_array.hpp"
#include "../../../utils/hashmap.hpp"

namespace bonk::macho {

enum relocation_type {
    RELOCATION_TYPE_CALL,
    RELOCATION_TYPE_CONSTANT
};

enum symbol_section {
    SYMBOL_SECTION_TEXT,
    SYMBOL_SECTION_DATA
};

struct macho_file {
    mach_header_64 header;
    segment_command_64 segment;
    section_64 section_data;
    section_64 section_text;
    symtab_command symtab;
    dysymtab_command dysymtab;

    hash_table<const char*, int> local_symbol_index_table;
    hash_table<const char*, int> external_symbol_index_table;

    dynamic_array<relocation_info> relocations;
    dynamic_array<uint32_t> local_symbol_string_indices;
    dynamic_array<uint32_t> external_symbol_string_indices;
    dynamic_array<nlist_64> local_symbols;
    dynamic_array<nlist_64> external_symbols;
    dynamic_array<const char*> string_table;
    dynamic_array<dynamic_array<char>*> text_fragments;
    dynamic_array<dynamic_array<char>*> data_fragments;

    static macho_file* create();
    void construct();

    void destruct();
    void release();

    void flush(FILE* file);
    bool add_relocation(uint32_t symbol, int32_t address, bool pc_rel, uint8_t data_length);
    bool add_code(dynamic_array<char>* code);
    bool add_data(dynamic_array<char>* data);

    void section_init_data();
    void section_init_text();
    void header_init();
    void symtab_init();
    void dysymtab_init();
    void segment_init();

    void declare_external_symbol(const char* symbol);
    void declare_internal_symbol(const char* symbol);

    void add_external_symbol(const char* symbol);
    void add_internal_symbol(const char* symbol, symbol_section section, uint32_t offset);

    char* add_underscore(const char*);

    const char* get_symbol_name(uint32_t symbol);
    uint32_t get_symbol_from_name(const char* symbol);
};

}

#endif //BONK_COMPILER_REINVENTED_MACHO_HPP
