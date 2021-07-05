
#include "draw_pd_128.h"

void mandelbrot_controller_on_draw_pd_128(s_mandelbrot_thread* thread) {
    s_mandelbrot_thread_cluster* cluster = thread->cluster;
    s_mandelbrot_controller* controller = cluster->controller;
    s_game* game = controller->super.game;
    s_proc_vm* vm = game->videomemory;

    int steps = controller->steps;

    double viewport_width = controller->zoom * (double)vm->window_width;
    double viewport_height = controller->zoom * (double)vm->window_height;

    double half_viewport_width = viewport_width / 2;
    double half_viewport_height = viewport_height / 2;

    double left_viewport_boundary = controller->camera_x - half_viewport_width;
    double x_step = viewport_width / (double) vm->real_window_width;

    double top_viewport_boundary = controller->camera_y + half_viewport_height;
    double y_step = viewport_height / (double) vm->real_window_height;

    double fractal_y = 0;
    double fractal_x = 0;

    long long viewport_y = vm->real_window_height - 1 - thread->thread_number;

    double radius = 10;
    double square_radius = radius * radius;

    __m128 square_radius_sse = _mm_set1_pd(square_radius);
    __m128 two_sse = _mm_set1_pd(2.0);
    __m128 shift_mask = _mm_set1_epi32(1);

    char* target = vm->framebuffer + thread->thread_number * vm->real_window_width * 4;

    for(; viewport_y >= 0; viewport_y -= cluster->thread_limit) {

        fractal_y = top_viewport_boundary - ((double)viewport_y * y_step);

        long long viewport_x = 0;

        char* target_saved = target;

        for(; viewport_x < vm->real_window_width; viewport_x += 2) {

            fractal_x = left_viewport_boundary + x_step * (double)viewport_x;

            __m128d loaded_x = _mm_set1_pd(0);
            __m128d loaded_y = _mm_set1_pd(0);
            __m128d loaded_x_square = loaded_x;
            __m128d loaded_y_square = loaded_y;

            __m128d fractal_x_sse = _mm_set_pd(fractal_x, fractal_x + x_step);
            __m128d fractal_y_sse = _mm_set1_pd(fractal_y);

            int i = 0;
            __v4su amount = _mm_set1_epi32(0);

            while(i++ < steps) {
                __m128d mixed = _mm_mul_pd(loaded_x, loaded_y);

                loaded_x = _mm_sub_pd(loaded_x_square, loaded_y_square);
                loaded_y = _mm_mul_pd(mixed, two_sse);

                loaded_x = _mm_add_pd(loaded_x, fractal_x_sse);
                loaded_y = _mm_add_pd(loaded_y, fractal_y_sse);

                loaded_x_square = _mm_mul_pd(loaded_x, loaded_x);
                loaded_y_square = _mm_mul_pd(loaded_y, loaded_y);

                __m128d distances = _mm_add_pd(loaded_x_square, loaded_y_square);

                __m128d mask = _mm_cmple_pd(distances, square_radius_sse);
                if(_mm_test_all_zeros(mask, shift_mask)) break;

                mask = _mm_and_ps(mask, shift_mask);
                amount = _mm_add_epi64(mask, amount);
            }

            uint32_t color1 = amount[3] * 4;
            uint32_t color2 = amount[0] * 4;

            color1 = color1 | color1 << 8 | color1 << 16;
            color2 = color2 | color2 << 8 | color2 << 16;

            *(uint32_t*)(target) = color1;
            *(uint32_t*)(target + 4) = color2;

            target += 8;
        }

        target = target_saved + vm->real_window_width * 4 * cluster->thread_limit;
    }
}