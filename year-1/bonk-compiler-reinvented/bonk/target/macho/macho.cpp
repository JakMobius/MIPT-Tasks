//
// Created by Темыч on 03.05.2021.
//

#include "macho.hpp"

namespace bonk::macho {

void macho_file::section_init_data() {
    strcpy(section_data.segname, SEG_DATA ); /* segname  <- __DATA */
    strcpy(section_data.sectname, SECT_DATA); /* sectname <- __data */
    section_data.addr          = 0;          /* = sectionText.size */
    section_data.size          = 0;          /* to be modified */
    section_data.offset        = 0;          /* = sectionText.offset */
                                             /*   + sectionText.size */
    section_data.align         = 1;          /* 2^1 code alignment */
    section_data.reloff        = 0;          /* no relocations in data section_data */
    section_data.nreloc        = 0;
    section_data.flags         = S_REGULAR;
}

void macho_file::section_init_text() {
    strcpy(section_text.segname, SEG_TEXT ); /* segname  <- __TEXT */
    strcpy(section_text.sectname, SECT_TEXT); /* sectname <- __text */
    section_text.addr          = 0;
    section_text.size          = 0;          /* to be modified */
    section_text.offset        = 0;          /* to be modified */
    section_text.align         = 4;          /* 2^4 code alignment */
    section_text.reloff        = segment.fileoff;
    section_text.nreloc        = 0;          /* to be modified */
    section_text.flags         = S_REGULAR | S_ATTR_PURE_INSTRUCTIONS | S_ATTR_SOME_INSTRUCTIONS;
}

void macho_file::header_init() {
    header.magic          = MH_MAGIC_64;
    header.cputype        = CPU_TYPE_X86_64;
    header.cpusubtype     = CPU_SUBTYPE_X86_64_ALL;
    header.filetype       = MH_OBJECT;
    header.ncmds          = 3;
    header.sizeofcmds     = sizeof(segment_command_64) +
                             sizeof(section_64) +
                             sizeof(section_64) +
                             sizeof(symtab_command) +
                             sizeof(dysymtab_command);
    header.flags          = MH_SUBSECTIONS_VIA_SYMBOLS;
}

void macho_file::symtab_init() {
    symtab.cmd            = LC_SYMTAB;
    symtab.cmdsize        = sizeof(symtab_command);
    symtab.symoff         = 0;       /* to be modified */
    symtab.nsyms          = 0;       /* to be modified */
    symtab.stroff         = 0;       /* to be modified */
    symtab.strsize        = 0;       /* to be modified */
}

void macho_file::dysymtab_init() {
    dysymtab.cmd          = LC_DYSYMTAB;
    dysymtab.cmdsize      = sizeof(dysymtab_command);
    dysymtab.ilocalsym    = 0; /* to be modified */
    dysymtab.nlocalsym    = 0; /* to be modified */
    dysymtab.iextdefsym   = 0; /* to be modified */
    dysymtab.nextdefsym   = 0; /* to be modified */
}

void macho_file::segment_init() {
    /*
     * Usually, as there is only one segment in the object file,
     * placing name is omitted.
     * strcpy(segment.segname, SEG_TEXT);
     */

    segment.cmd                = LC_SEGMENT_64;
    segment.cmdsize            = sizeof(segment) + 2 * sizeof(section_64);
    segment.vmaddr             = 0;
    segment.vmsize             = 0; /* to be modified */
    segment.fileoff            = header.sizeofcmds + sizeof(mach_header_64); /* to be modified */
    segment.filesize           = 0; /* to be modified */
    segment.maxprot            = VM_PROT_READ | VM_PROT_EXECUTE;
    segment.initprot           = VM_PROT_READ | VM_PROT_EXECUTE;
    segment.nsects             = 2; /* code and data sections */
}

macho_file* macho_file::create() {
    DEFAULT_ALLOC_CONSTRUCT(macho_file);
}

void macho_file::construct() {
    header_init();
    segment_init();
    symtab_init();
    dysymtab_init();
    section_init_text();
    section_init_data();

    relocations.construct();
    local_symbols.construct();
    external_symbols.construct();
    string_table.construct();
    text_fragments.construct();
    data_fragments.construct();
    local_symbol_index_table.construct(16);
    external_symbol_index_table.construct(16);
    local_symbol_string_indices.construct();
    external_symbol_string_indices.construct();
}

void macho_file::destruct() {
    for(int i = 0; i < text_fragments.size; i++) text_fragments.get(i)->release();
    for(int i = 0; i < data_fragments.size; i++) data_fragments.get(i)->release();
    for(int i = 0; i < string_table.size; i++)   free((void*) string_table.get(i));

    relocations.destruct();
    local_symbols.destruct();
    external_symbols.destruct();
    string_table.destruct();
    text_fragments.destruct();
    data_fragments.destruct();
    local_symbol_index_table.destruct();
    external_symbol_index_table.destruct();
    external_symbol_string_indices.destruct();
}

void macho_file::release() {
    destruct();
    free(this);
}

bool macho_file::add_code(dynamic_array<char>* code) {
    if(text_fragments.push(code)) return false;
    section_text.size += code->size;

    return true;
}

bool macho_file::add_data(dynamic_array<char>* data) {
    if(data_fragments.push(data)) return false;
    section_data.size += data->size;

    return true;
}

void macho_file::declare_external_symbol(const char* symbol) {
    symbol = add_underscore(symbol);
    string_table.push(symbol);
    external_symbol_index_table.insert(symbol + 1, external_symbols.size);
    external_symbols.push({
        .n_un = { symtab.strsize }
    });
    external_symbol_string_indices.push(symtab.nsyms);
    symtab.strsize += strlen(symbol) + 1; // don't forget trailing zero byte
    symtab.nsyms++;
    dysymtab.nextdefsym++;
}

void macho_file::declare_internal_symbol(const char* symbol) {
    symbol = add_underscore(symbol);
    string_table.push(symbol);
    local_symbol_index_table.insert(symbol + 1, local_symbols.size);
    local_symbols.push({
        .n_un = { symtab.strsize }
    });
    local_symbol_string_indices.push(symtab.nsyms);
    symtab.strsize += strlen(symbol) + 1; // don't forget trailing zero byte
    symtab.nsyms++;
    dysymtab.nlocalsym++;
    dysymtab.iextdefsym++;
}

void macho_file::add_external_symbol(const char* symbol) {

    auto i = external_symbol_index_table.get(symbol);

    external_symbols.set(i, {
        external_symbols.get(i).n_un,
        N_UNDF | N_EXT,
        NO_SECT,
        REFERENCE_FLAG_UNDEFINED_NON_LAZY,
        0
    });
}

void macho_file::add_internal_symbol(const char* symbol, symbol_section section, uint32_t offset) {
    uint8_t section_index = 0;

    switch(section) {
        case SYMBOL_SECTION_TEXT: section_index = 1; break;
        case SYMBOL_SECTION_DATA: section_index = 2; break;
    }

    auto i = local_symbol_index_table.get(symbol);

    local_symbols.set(i, {
        local_symbols.get(i).n_un,
        N_SECT | N_EXT,
        section_index,
        REFERENCE_FLAG_DEFINED,
        offset
    });
}

uint32_t macho_file::get_symbol_from_name(const char* symbol) {
    if(local_symbol_index_table.has(symbol)) {
        return local_symbol_index_table.get(symbol) + dysymtab.ilocalsym;
    } else if(external_symbol_index_table.has(symbol)) {
        return external_symbol_index_table.get(symbol) + dysymtab.iextdefsym;
    } else {
        assert(!"Symbol was not declared");
    }
}

const char* macho_file::get_symbol_name(uint32_t symbol) {
    if(symbol >= dysymtab.ilocalsym && symbol < dysymtab.ilocalsym + dysymtab.nlocalsym) {
        return string_table.get(local_symbol_string_indices.get(symbol - dysymtab.ilocalsym));
    } else if(symbol >= dysymtab.iextdefsym && symbol < dysymtab.iextdefsym + dysymtab.nextdefsym) {
        return string_table.get(external_symbol_string_indices.get(symbol - dysymtab.iextdefsym));
    } else {
        assert(!"Invalid symbol");
    }
}

bool macho_file::add_relocation(uint32_t symbol, int32_t address, bool pc_rel, uint8_t data_length) {

    relocation_info relocation = {};

    relocation.r_symbolnum = symbol;
    relocation.r_address = address;
    relocation.r_pcrel = pc_rel;

    switch(data_length) {
        case 1: relocation.r_length = 0; break;
        case 2: relocation.r_length = 1; break;
        case 4: relocation.r_length = 2; break;
        case 8: relocation.r_length = 3; break;
        default: assert(!"Invalid data length");
    }

    relocation.r_type = GENERIC_RELOC_SECTDIFF;
    relocation.r_extern = 1;

    if(relocations.push(relocation)) return false;
    section_text.nreloc++;
    return true;
}


void macho_file::flush(FILE* file) {

    segment.filesize = section_text.size + section_data.size;
    segment.vmsize = segment.filesize;
    section_text.reloff = segment.fileoff + segment.filesize;
    section_text.offset = segment.fileoff;
    section_data.offset = segment.fileoff + section_text.size;
    section_data.addr = section_text.size;
    symtab.symoff = section_text.reloff + section_text.nreloc * sizeof(relocation_info);
    symtab.stroff = symtab.symoff + symtab.nsyms * sizeof(nlist_64);

    // Write header
    fwrite(&header, 1, sizeof(mach_header_64), file);

    // Write segment
    fwrite(&segment, 1, sizeof(segment_command_64), file);

    // Write section text
    fwrite(&section_text, 1, sizeof(section_64), file);

    // Write section data
    fwrite(&section_data, 1, sizeof(section_64), file);

    // Write symtab
    fwrite(&symtab, 1, sizeof(symtab_command), file);

    // Write dysymtab
    fwrite(&dysymtab, 1, sizeof(dysymtab_command), file);

    // Write code
    for(int i = 0; i < text_fragments.size; i++) {
        auto fragment = text_fragments.get(i);
        fwrite(fragment->point(0), 1, fragment->size, file);
    }

    // Write data
    for(int i = 0; i < data_fragments.size; i++) {
        auto fragment = data_fragments.get(i);
        fwrite(fragment->point(0), 1, fragment->size, file);
    }

    // Write relocations
    for(int i = 0; i < relocations.size; i++) {
        auto relocation = relocations.point(i);
        fwrite(relocation, 1, sizeof(*relocation), file);
    }

    // Write local symbol table
    for(int i = 0; i < local_symbols.size; i++) {
        auto symbol = local_symbols.point(i);
        if(symbol->n_sect == 2) symbol->n_value += section_text.size;
        fwrite(symbol, 1, sizeof(*symbol), file);
        if(symbol->n_sect == 2) symbol->n_value -= section_text.size;
    }

    // Write external symbol table
    for(int i = 0; i < external_symbols.size; i++) {
        auto symbol = external_symbols.point(i);
        if(symbol->n_sect == 2) symbol->n_value += section_text.size;
        fwrite(symbol, 1, sizeof(*symbol), file);
        if(symbol->n_sect == 2) symbol->n_value -= section_text.size;
    }

    // Write string table
    for(int i = 0; i < string_table.size; i++) {
        auto str = string_table.get(i);
        fprintf(file, "%s", str);
        fputc(0, file);
    }
}

char* macho_file::add_underscore(const char* symbol) {
    int length = strlen(symbol);
    char* buffer = (char*)malloc(length + 2);
    if(!buffer) return nullptr;
    buffer[0] = '_';
    memcpy(buffer + 1, symbol, length + 1);
    return buffer;
}

}