cmake_minimum_required(VERSION 3.17)
project(fast_hash_table_clion CXX ASM)

enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_OBJECT_FORMAT macho64)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> \
    <FLAGS> -f ${CMAKE_ASM_NASM_OBJECT_FORMAT} -o <OBJECT> <SOURCE>")

set_source_files_properties(asm.asm PROPERTIES COMPILE_FLAGS "-g -f macho64 -F dwarf")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS -O3)
set(CMAKE_CXX_FLAGS -O3)
#set(CMAKE_CXX_FLAGS -march=native)
#set(CMAKE_CXX_FLAGS -fasm-blocks)
set(ASM_OPTIONS "-x assembler-with-cpp")
set(CMAKE_ASM_FLAGS "${CFLAGS} ${ASM_OPTIONS}" )

add_executable(fast_hash_table_clion asm.asm main.cpp list.cpp hash-table.cpp file_reader.cpp file_reader.hpp dynamic_array.cpp dynamic_array.hpp benchmarks.cpp benchmarks.h cli.cpp cli.h)