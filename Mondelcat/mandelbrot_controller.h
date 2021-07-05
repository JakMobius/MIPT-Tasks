
#ifndef MANDELBROT_MANDELBROT_CONTROLLER_H
#define MANDELBROT_MANDELBROT_CONTROLLER_H

typedef struct mandelbrot_controller s_mandelbrot_controller;
typedef struct mandelbrot_thread s_mandelbrot_thread;

#include "game.h"
#include "mandelbrot_methods/draw_nosse.h"
#include "mandelbrot_methods/draw_pd_128_messed.h"
#include "mandelbrot_methods/draw_pd_128.h"
#include "mandelbrot_methods/draw_ps_128.h"
#include "mandelbrot_thread.h"
#include <pthread.h>
#include <semaphore.h>

typedef struct mandelbrot_controller {
    s_game_controller super;

    float saved_zoom;
    float saved_camera_x;
    float saved_camera_y;
    s_game_controller* switch_to;
    double zoom;
    double camera_y;
    double camera_x;
    int old_mouse_x;
    int old_mouse_y;
    bool mouse_clicked;
    bool need_redraw;
    int function_index;
    int steps;

    s_mandelbrot_thread_cluster* cluster;

    void (*draw_function)(s_mandelbrot_thread* thread);
} s_mandelbrot_controller;

s_mandelbrot_controller* mandelbrot_controller_create();
void mandelbrot_controller_destroy(s_mandelbrot_controller* controller);
void mandelbrot_controller_release(s_mandelbrot_controller* controller);
void mandelbrot_controller_on_enable(s_mandelbrot_controller* controller);
void mandelbrot_controller_on_disable(s_mandelbrot_controller* controller);
bool mandelbrot_controller_on_event(s_mandelbrot_controller* controller, SDL_Event* event);
void mandelbrot_controller_on_tick(s_mandelbrot_controller* controller);
void mandelbrot_controller_on_draw(s_mandelbrot_controller* controller);
void mandelbrot_controller_update_function(s_mandelbrot_controller* controller);
void mandelbrot_controller_benchmark(s_mandelbrot_controller* controller);

#endif //MANDELBROT_MANDELBROT_CONTROLLER_H
