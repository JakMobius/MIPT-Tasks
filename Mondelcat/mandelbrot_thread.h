
#ifndef MANDELBROT_MANDELBROT_THREAD_H
#define MANDELBROT_MANDELBROT_THREAD_H

typedef struct mandelbrot_thread_cluster s_mandelbrot_thread_cluster;

#include "mandelbrot_controller.h"
#include "semaphores.h"

typedef struct mandelbrot_thread {
    s_mandelbrot_thread_cluster* cluster;
    int thread_number;
    bool running;
    s_rk_sema start_semaphore;
    s_rk_sema finish_semaphore;
    pthread_t thread_id;
} s_mandelbrot_thread;

typedef struct mandelbrot_thread_cluster {
    s_mandelbrot_thread** threads;
    int thread_amount;
    s_mandelbrot_controller* controller;
    int thread_limit;
} s_mandelbrot_thread_cluster;

s_mandelbrot_thread_cluster* mandelbrot_cluster_create(int threads, s_mandelbrot_controller * cluster);
void mandelbrot_cluster_destroy(s_mandelbrot_thread_cluster* cluster);
void mandelbrot_cluster_release(s_mandelbrot_thread_cluster* cluster);
void mandelbrot_cluster_perform(s_mandelbrot_thread_cluster* cluster, int thread_limit);

s_mandelbrot_thread* mandelbrot_thread_create(int id, s_mandelbrot_thread_cluster* cluster);

_Noreturn void* mandelbrot_thread_task(void* arg);
void mandelbrot_thread_wait(s_mandelbrot_thread* thread);
void mandelbrot_thread_perform(s_mandelbrot_thread* thread);
void mandelbrot_thread_destroy(s_mandelbrot_thread* thread);
void mandelbrot_thread_release(s_mandelbrot_thread* thread);

#endif //MANDELBROT_MANDELBROT_THREAD_H
