//
// Created by Артем on 10.09.2021.
//

#include "renderer.hpp"

Renderer::Renderer(draw_function_type draw_function, int thread_count, void* user_data):
                                            draw_function(draw_function),
                                            thread_count(thread_count),
                                            threads(thread_count),
                                            user_data(user_data) {
    for(int i = 0; i < thread_count; i++) {
        threads.push(new RendererTask(i, this));
    }
}

Renderer::~Renderer() {
    for(int i = 0; i < thread_count; i++) {
        if(threads.get(i)) {
            delete threads.get(i);
        }
    }
}
void Renderer::perform() {
    for(int i = 0; i < thread_count; i++) {
        threads.get(i)->perform();
    }

    for(int i = 0; i < thread_count; i++) {
        threads.get(i)->wait();
    }
}

static void* run_renderer_task(void* arg) { ((RendererTask*)arg)->task(); return nullptr; }

RendererTask::RendererTask(int id, Renderer* renderer): id(id), renderer(renderer) {
    rk_sema_init(&start_semaphore, 0);
    rk_sema_init(&finish_semaphore, 0);
    pthread_create(&thread_id, nullptr, run_renderer_task, this);
}

RendererTask::~RendererTask() {
    pthread_kill(thread_id, 9);
}

_Noreturn void RendererTask::task() {

    while(true) {
        rk_sema_wait(&start_semaphore);
        renderer->get_draw_function()(this);
        rk_sema_post(&finish_semaphore);
    }
}

void RendererTask::perform() {
    rk_sema_post(&start_semaphore);
}

void RendererTask::wait() {
    rk_sema_wait(&finish_semaphore);
}