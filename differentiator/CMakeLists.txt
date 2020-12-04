cmake_minimum_required(VERSION 3.17)
project(differentiator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")

add_executable(differentiator main.c tree.c tree_serializer.c simplifier.c grapher.c parsing/parser.c parsing/grammatics/grammatic_math_expression.c parsing/grammatics/grammatic_global.c parsing/grammatics/grammatic_expression.c parsing/grammatics/grammatic_math_expression.c parsing/grammatics/grammatic_math_factor.c parsing/grammatics/grammatic_math_term.c parsing/grammatics/grammatic_number.c parsing/grammatics/grammatic_global.c parsing/grammatics/grammatic_reference.c compiler.c errors.c parsing/grammatics/grammatic_power_base.c parsing/grammatics/grammatic_power_base.h parsing/grammatics/grammatic_derivative.c parsing/grammatics/grammatic_derivative.h)