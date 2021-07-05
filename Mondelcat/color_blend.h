
#ifndef MANDELBROT_COLOR_BLEND_H
#define MANDELBROT_COLOR_BLEND_H

#include <emmintrin.h>
#include <immintrin.h>
#include <stdbool.h>
#include <x86intrin.h>
#include "videomemory.h"
#include "bmp_reader.h"

static const unsigned char SZ = 0x80;

extern __m128 blend_first_batch_load_bmp_mask;
extern __m128 blend_second_batch_load_bmp_mask;
extern __m128 blend_first_batch_store_mask;
extern __m128 blend_second_batch_store_mask;
extern __m128 blend_first_batch_load_vm_mask;
extern __m128 blend_second_batch_load_vm_mask;
extern __m128 blend_load_alpha_mask;
extern __m128 blend_alpha_max_mask;

void init_blend_constants();

void _mm_print_epi8(__m128 a);
void write_bmp_to_vm(s_proc_vm* vm, char* bmp, s_bitmap_info_header* bmp_info, int draw_x, int draw_y);
void blend_bmp_to_vm(s_proc_vm* vm, char* bmp, s_bitmap_info_header* bmp_info, int draw_x, int draw_y);

#endif //MANDELBROT_COLOR_BLEND_H
