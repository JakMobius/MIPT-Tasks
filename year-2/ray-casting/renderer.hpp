#pragma once

class Renderer;
struct RendererTask;

#include "dynamic_array.hpp"
#include "semaphores.h"
#include "pthread.h"

class RendererTask {
    int id;
    Renderer* renderer;
    struct rk_sema start_semaphore;
    struct rk_sema finish_semaphore;
    pthread_t thread_id;
public:

    RendererTask(int i, Renderer* renderer);
    ~RendererTask();

    void task();
    void perform();
    void wait();

    int get_id() { return id; }
    Renderer* get_renderer() { return renderer; }
};

typedef void (*draw_function_type)(RendererTask* thread);

class Renderer {
    dynamic_array<RendererTask*> threads;
    draw_function_type draw_function;
    int thread_count;
    void* user_data;

public:
    explicit Renderer(draw_function_type draw_function, int thread_count, void* user_data = nullptr);
    ~Renderer();

    draw_function_type get_draw_function() const { return draw_function; };
    int get_thread_count() const { return thread_count; }
    void* get_user_data() const { return user_data; }
    void perform();
};