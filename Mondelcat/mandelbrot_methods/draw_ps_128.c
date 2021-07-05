
#include "draw_ps_128.h"

void mandelbrot_controller_on_draw_ps_128(s_mandelbrot_thread* thread) {
    s_mandelbrot_thread_cluster* cluster = thread->cluster;
    s_mandelbrot_controller* controller = cluster->controller;
    s_game* game = controller->super.game;
    s_proc_vm* vm = game->videomemory;

    int steps = controller->steps;

    float viewport_width = (float)controller->zoom * (float)vm->window_width;
    float viewport_height = (float)controller->zoom * (float)vm->window_height;

    float half_viewport_width = viewport_width / 2;
    float half_viewport_height = viewport_height / 2;

    float left_viewport_boundary = (float)controller->camera_x - half_viewport_width;
    float x_step = viewport_width / (float) vm->real_window_width;

    float top_viewport_boundary = (float)controller->camera_y + half_viewport_height;
    float y_step = viewport_height / (float) vm->real_window_height;

    float fractal_y = 0;
    float fractal_x = 0;
    long long viewport_y = vm->real_window_height - 1 - thread->thread_number;

    float radius = 10;
    float square_radius = radius * radius;

    __m128 square_radius_sse = _mm_set1_ps(square_radius);
    __m128 two_sse = _mm_set1_ps((float)2.0);
    __m128 shift_mask = _mm_set1_epi32(1);

    __m128 offsets = _mm_set_ps(0, x_step, x_step * 2, x_step * 3);

    char* target = vm->framebuffer + thread->thread_number * vm->real_window_width * 4;

    for(; viewport_y >= 0; viewport_y -= cluster->thread_limit) {

        fractal_y = top_viewport_boundary - ((float)viewport_y * y_step);

        long long viewport_x = 0;

        char* target_saved = target;

        for(; viewport_x < vm->real_window_width; viewport_x += 4) {

            fractal_x = left_viewport_boundary + x_step * (float)viewport_x;

            __m128 complex_x = _mm_set1_ps(0);
            __m128 complex_y = _mm_set1_ps(0);
            __m128 complex_x_square = complex_x;
            __m128 complex_y_square = complex_y;

            __m128 fractal_x_sse = _mm_set1_ps(fractal_x);
            __m128 fractal_y_sse = _mm_set1_ps(fractal_y);

            fractal_x_sse = _mm_add_ps(offsets, fractal_x_sse);

            int i = 0;
            __v4su amount = _mm_set1_epi32(0);

            while(i++ < steps) {
                __m128d mixed = _mm_mul_ps(complex_x, complex_y);

                complex_x = _mm_sub_ps(complex_x_square, complex_y_square);
                complex_y = _mm_mul_ps(mixed, two_sse);

                complex_x = _mm_add_ps(complex_x, fractal_x_sse);
                complex_y = _mm_add_ps(complex_y, fractal_y_sse);

                complex_x_square = _mm_mul_ps(complex_x, complex_x);
                complex_y_square = _mm_mul_ps(complex_y, complex_y);

                __m128 distances = _mm_add_ps(complex_x_square, complex_y_square);

                __m128 mask = _mm_cmple_ps(distances, square_radius_sse);
                if(_mm_test_all_zeros(mask, shift_mask)) break;

                mask = _mm_and_ps(mask, shift_mask);
                amount = _mm_add_epi32(mask, amount);
            }

            uint32_t color1 = amount[3] * 4;
            uint32_t color2 = amount[2] * 4;
            uint32_t color3 = amount[1] * 4;
            uint32_t color4 = amount[0] * 4;

            color1 = color1 | color1 << 8 | color1 << 16;
            color2 = color2 | color2 << 8 | color2 << 16;
            color3 = color3 | color3 << 8 | color3 << 16;
            color4 = color4 | color4 << 8 | color4 << 16;

            *(uint32_t*)(target) = color1;
            *(uint32_t*)(target + 4) = color2;
            *(uint32_t*)(target + 8) = color3;
            *(uint32_t*)(target + 12) = color4;

            target += 16;
        }

        target = target_saved + vm->real_window_width * 4 * cluster->thread_limit;
    }
}