
#include "color_blend.h"

static long long min(long long a, long long b) {
    return a < b ? a : b;
}

static long long max(long long a, long long b) {
    return a > b ? a : b;
}

__m128 blend_first_batch_load_bmp_mask;
__m128 blend_second_batch_load_bmp_mask;
__m128 blend_first_batch_store_mask;
__m128 blend_second_batch_store_mask;
__m128 blend_first_batch_load_vm_mask;
__m128 blend_second_batch_load_vm_mask;
__m128 blend_load_alpha_mask;
__m128 blend_alpha_max_mask;
bool blend_constants_initialized = false;

void _mm_print_epi8(__m128 a) {
    char data[16];
    _mm_store_ps((float*)data, a);
    for(int i = 0; i < 16; i++) {
        printf("%3d ", (unsigned char)data[i]);
    }
    printf("\n");
}

static inline __m128 blend_four_bmp_colors(__m128 bmp_foreground, __m128 vm_background) {
    __m128 first_batch = _mm_shuffle_epi8(bmp_foreground, blend_first_batch_load_bmp_mask);
    __m128 second_batch = _mm_shuffle_epi8(bmp_foreground, blend_second_batch_load_bmp_mask);

    __m128 alpha_first = _mm_shuffle_epi8(first_batch, blend_load_alpha_mask);
    __m128 alpha_second = _mm_shuffle_epi8(second_batch, blend_load_alpha_mask);

    first_batch = _mm_mullo_epi16(first_batch, alpha_first);
    second_batch = _mm_mullo_epi16(second_batch, alpha_second);

    bmp_foreground = _mm_shuffle_epi8(first_batch, blend_first_batch_store_mask);
    second_batch = _mm_shuffle_epi8(second_batch, blend_second_batch_store_mask);
    __m128 result = _mm_add_epi8(bmp_foreground, second_batch);

    first_batch = _mm_shuffle_epi8(vm_background, blend_first_batch_load_vm_mask);
    second_batch = _mm_shuffle_epi8(vm_background, blend_second_batch_load_vm_mask);

    alpha_first = _mm_sub_epi8(blend_alpha_max_mask, alpha_first);
    alpha_second = _mm_sub_epi8(blend_alpha_max_mask, alpha_second);

    first_batch = _mm_mullo_epi16(first_batch, alpha_first);
    second_batch = _mm_mullo_epi16(second_batch, alpha_second);

    bmp_foreground = _mm_shuffle_epi8(first_batch, blend_first_batch_store_mask);
    second_batch = _mm_shuffle_epi8(second_batch, blend_second_batch_store_mask);

    result = _mm_add_epi8(result, second_batch);
    return _mm_add_epi8(result, bmp_foreground);
}

void write_bmp_to_vm(s_proc_vm* vm, char* bmp, s_bitmap_info_header* bmp_info, int draw_x, int draw_y) {

    int bmp_width = bmp_info->biWidth;
    int bmp_height = bmp_info->biHeight;

    char* videomemory = vm->framebuffer;

    __m128 shuffle_mask = _mm_set_epi8(15, 12, 13, 14,
                                       11, 8, 9, 10,
                                       7,  4, 5, 6,
                                       3,  0, 1, 2);

    int draw_x_start_index = max(0, -draw_x);
    int draw_y_start_index = max(0, -draw_y);

    int min_draw_y = max(0, draw_y);
    int min_draw_x = max(0, draw_x);
    int max_draw_width = min(bmp_width, vm->window_width - min_draw_x);
    int max_draw_height = min(bmp_height, vm->window_height - min_draw_y);

    unsigned long vm_width = vm->window_width;
    char* target = videomemory + (min_draw_x + min_draw_y * vm_width) * 4;
    char* source = bmp;

    for(int y = draw_y_start_index; y < max_draw_height; y++) {
        char* target_line_start = target;
        char* source_line_start = source;

        // Drawing 4-pixel batches

        int x = draw_x_start_index;
        for(; x <= max_draw_width - 4; x += 4) {

            __m128 four_pixels = _mm_lddqu_si128((const __m128i*) source);
            __m128 shuffled = _mm_shuffle_epi8(four_pixels, shuffle_mask);
            _mm_storeu_ps((float*) target, shuffled);

            source += 16;
            target += 16;
        }

        // Drawing last 0-3 pixels if they exist

        target--;
        while(x++ < max_draw_width) {
            *(int*)target = _bswap(*(int*)source);

            source += 4;
            target += 4;
        }

        source = source_line_start + bmp_width * 4;
        target = target_line_start + vm_width * 4;
    }
}

void blend_bmp_to_vm(s_proc_vm* vm, char* bmp, s_bitmap_info_header* bmp_info, int draw_x, int draw_y) {

    int bmp_width = bmp_info->biWidth;
    int bmp_height = bmp_info->biHeight;

    char* videomemory = vm->framebuffer;

    int draw_x_start_index = max(0, -draw_x);
    int draw_y_start_index = max(0, -draw_y);

    int min_draw_y = max(0, draw_y);
    int min_draw_x = max(0, draw_x);
    int max_draw_width = min(bmp_width, vm->window_width - min_draw_x);
    int max_draw_height = min(bmp_height, vm->window_height - min_draw_y);

    unsigned long vm_width = vm->window_width;
    char* target = videomemory + (min_draw_x + min_draw_y * vm_width) * 4;
    char* source = (char*)bmp + (draw_x_start_index + draw_y_start_index * bmp_width) * 4;

    int excessive_pixels = (max_draw_width - draw_x_start_index) & 0b11;

    __m128 positive_mask = _mm_set1_epi8(-1);
    __m128 last_fragment_mask = positive_mask;

    if((excessive_pixels & 2) == 0) last_fragment_mask = _mm_srli_si128(last_fragment_mask, 8);
    if((excessive_pixels & 1) == 0) last_fragment_mask = _mm_srli_si128(last_fragment_mask, 4);

    __m128 inverse_last_fragment_mask = _mm_xor_ps(last_fragment_mask, positive_mask);

    for(int y = draw_y_start_index; y < max_draw_height; y++) {
        char* target_line_start = target;
        char* source_line_start = source;

        // Drawing 4-pixel batches

        for(int x = draw_x_start_index; x <= max_draw_width - 4; x += 4) {
            _mm_storeu_ps((float*) target, blend_four_bmp_colors(
                    _mm_lddqu_si128((__m128i*) source),
                    _mm_lddqu_si128((__m128i*) target))
            );

            source += 16;
            target += 16;
        }

        // Drawing 0-3 remaining pixels

        if(excessive_pixels) {
            __m128 last_fragment_source = _mm_lddqu_si128((__m128i*) source);
            __m128 last_fragment_target = _mm_lddqu_si128((__m128i*) target);
            __m128 last_fragment_blended = blend_four_bmp_colors(last_fragment_source, last_fragment_target);

            last_fragment_blended = _mm_and_ps(last_fragment_blended, last_fragment_mask);
            last_fragment_target =  _mm_and_ps(last_fragment_target, inverse_last_fragment_mask);

            last_fragment_target = _mm_add_epi8(last_fragment_blended, last_fragment_target);

            _mm_storeu_ps((float*) target, last_fragment_target);
        }

        source = source_line_start + bmp_width * 4;
        target = target_line_start + vm_width * 4;
    }
}

void init_blend_constants() {
    if(blend_constants_initialized) return;
    blend_constants_initialized = true;

    blend_first_batch_load_bmp_mask = _mm_set_epi8(
            SZ, 15, SZ, 12,
            SZ, 13, SZ, 14,
            SZ, 11, SZ, 8,
            SZ, 9,  SZ, 10
    );

    blend_second_batch_load_bmp_mask = _mm_set_epi8(
            SZ, 7, SZ, 4,
            SZ, 5, SZ, 6,
            SZ, 3, SZ, 0,
            SZ, 1, SZ, 2
    );

    blend_first_batch_store_mask = _mm_set_epi8(
            15, 13, 11, 9,
            7,  5,  3,  1,
            SZ, SZ, SZ, SZ,
            SZ, SZ, SZ, SZ
    );

    blend_second_batch_store_mask = _mm_set_epi8(
            SZ, SZ, SZ, SZ,
            SZ, SZ, SZ, SZ,
            15, 13, 11, 9,
            7,  5,  3,  1
    );

    blend_first_batch_load_vm_mask = _mm_set_epi8(
            SZ, 15, SZ, 14,
            SZ, 13, SZ, 12,
            SZ, 11, SZ, 10,
            SZ, 9,  SZ, 8
    );

    blend_second_batch_load_vm_mask = _mm_set_epi8(
            SZ, 7, SZ, 6,
            SZ, 5, SZ, 4,
            SZ, 3, SZ, 2,
            SZ, 1, SZ, 0
    );

    blend_load_alpha_mask = _mm_set_epi8(
            SZ, 14,  SZ, 14,
            SZ, 14,  SZ, 14,
            SZ, 6,  SZ, 6,
            SZ, 6,  SZ, 6
    );

    blend_alpha_max_mask = _mm_set_epi8(
            0, 0xFF, 0, 0xFF,
            0, 0xFF, 0, 0xFF,
            0, 0xFF, 0, 0xFF,
            0, 0xFF, 0, 0xFF
    );
}