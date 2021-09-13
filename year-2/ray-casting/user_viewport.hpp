#pragma once

#include <SFML/Config.hpp>
#include "camera.hpp"
#include "renderer.hpp"

struct UserViewport {
    int window_width;
    int window_height;
    sf::Uint8* pixels;
    Camera* camera;
    World* world;

    static void draw(RendererTask* task) {
        UserViewport* viewport = (UserViewport*) task->get_renderer()->get_user_data();
        viewport->next_frame(task->get_id(), task->get_renderer()->get_thread_count());
    }

private:
    void next_frame(int thread_id, int total_threads) {
        camera->trace_rays(world, (uint32_t*)(pixels), window_width, window_height, thread_id, total_threads);
    }
};