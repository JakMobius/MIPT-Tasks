
#include "mandelbrot_thread.h"

s_mandelbrot_thread_cluster* mandelbrot_cluster_create(int threads, s_mandelbrot_controller* controller) {
    s_mandelbrot_thread_cluster* cluster = calloc(1, sizeof(s_mandelbrot_controller));
    if(!cluster) return NULL;

    cluster->thread_amount = threads;
    cluster->controller = controller;
    cluster->threads = calloc(threads, sizeof(s_mandelbrot_thread*));
    if(!cluster->threads) {
        mandelbrot_cluster_release(cluster);
        return NULL;
    }

    for(int i = 0; i < cluster->thread_amount; i++) {
        cluster->threads[i] = mandelbrot_thread_create(i, cluster);
    }

    return cluster;
}

void mandelbrot_cluster_destroy(s_mandelbrot_thread_cluster* cluster) {
    if(cluster->threads) {
        for(int i = 0; i < cluster->thread_amount; i++) {
            if(cluster->threads[i]) {
                mandelbrot_thread_release(cluster->threads[i]);
            }
        }
        free(cluster->threads);
    }
}

void mandelbrot_cluster_release(s_mandelbrot_thread_cluster* cluster) {
    mandelbrot_cluster_destroy(cluster);
}

void mandelbrot_cluster_perform(s_mandelbrot_thread_cluster* cluster, int thread_limit) {
    if(thread_limit == -1) thread_limit = cluster->thread_amount;
    cluster->thread_limit = thread_limit;

    for(int i = 0; i < thread_limit && i < cluster->thread_amount; i++) {
        mandelbrot_thread_perform(cluster->threads[i]);
    }

    for(int i = 0; i < thread_limit && i < cluster->thread_amount; i++) {
        mandelbrot_thread_wait(cluster->threads[i]);
    }
}

s_mandelbrot_thread* mandelbrot_thread_create(int id, s_mandelbrot_thread_cluster* cluster) {
    s_mandelbrot_thread* thread = calloc(1, sizeof(s_mandelbrot_controller));
    if(!thread) return NULL;

    thread->cluster = cluster;
    thread->thread_number = id;
    rk_sema_init(&thread->start_semaphore, 0);
    rk_sema_init(&thread->finish_semaphore, 0);
    pthread_create(&thread->thread_id, NULL, mandelbrot_thread_task, thread);

    return thread;
}

_Noreturn void* mandelbrot_thread_task(void* arg) {
    s_mandelbrot_thread* thread = (s_mandelbrot_thread*) arg;

    while(true) {
        rk_sema_wait(&thread->start_semaphore);
        thread->cluster->controller->draw_function(thread);
        rk_sema_post(&thread->finish_semaphore);
    }
}

void mandelbrot_thread_perform(s_mandelbrot_thread* thread) {
    rk_sema_post(&thread->start_semaphore);
}

void mandelbrot_thread_wait(s_mandelbrot_thread* thread) {
    rk_sema_wait(&thread->finish_semaphore);
}

void mandelbrot_thread_destroy(s_mandelbrot_thread* thread) {
    pthread_kill(thread->thread_id, 9);
}

void mandelbrot_thread_release(s_mandelbrot_thread* thread) {
    mandelbrot_thread_destroy(thread);
    free(thread);
}