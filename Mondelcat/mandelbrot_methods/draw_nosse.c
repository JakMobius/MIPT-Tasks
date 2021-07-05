
#include "draw_nosse.h"

void mandelbrot_controller_on_draw_nosse(s_mandelbrot_thread* thread) {
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

    char* target = vm->framebuffer + thread->thread_number * vm->real_window_width * 4;

    for(; viewport_y >= 0; viewport_y -= cluster->thread_limit) {

        fractal_y = top_viewport_boundary - ((double)viewport_y * y_step);

        long long viewport_x = 0;

        char* target_saved = target;

        for(; viewport_x < vm->real_window_width; viewport_x += 1) {

            fractal_x = left_viewport_boundary + x_step * (double)viewport_x;

            double complex_x = 0;
            double complex_y = 0;

            int i = 0;

            while(i < steps) {
                i++;

                double new_complex_x = complex_x * complex_x - complex_y * complex_y + fractal_x;
                double new_complex_y = complex_x * complex_y * 2 + fractal_y;

                complex_x = new_complex_x;
                complex_y = new_complex_y;

                if(complex_x * complex_x + complex_y * complex_y > square_radius) break;
            }

            target[0] = i * 4;
            target[1] = i * 4;
            target[2] = i * 4;

            target += 4;
        }

        target = target_saved + vm->real_window_width * 4 * cluster->thread_limit;
    }
}