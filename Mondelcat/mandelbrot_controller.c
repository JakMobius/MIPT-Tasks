
#include "mandelbrot_controller.h"

static long long min(long long a, long long b) {
    return a < b ? a : b;
}

static long long max(long long a, long long b) {
    return a > b ? a : b;
}

s_mandelbrot_controller* mandelbrot_controller_create() {
    s_mandelbrot_controller* controller = calloc(1, sizeof(s_mandelbrot_controller));
    if(!controller) return NULL;

    controller->super.on_draw = (void (*)(s_game_controller*)) mandelbrot_controller_on_draw;
    controller->super.on_tick = (void (*)(s_game_controller*)) mandelbrot_controller_on_tick;
    controller->super.on_enable = (void (*)(s_game_controller*)) mandelbrot_controller_on_enable;
    controller->super.on_disable = (void (*)(s_game_controller*)) mandelbrot_controller_on_disable;
    controller->super.on_event = (bool (*)(s_game_controller*, SDL_Event*)) mandelbrot_controller_on_event;

    controller->zoom = 1.0;
    controller->camera_x = 0;
    controller->camera_y = 0;
    controller->need_redraw = true;

    controller->cluster = mandelbrot_cluster_create(16, controller);
    controller->function_index = 0;
    controller->steps = 50;
    mandelbrot_controller_update_function(controller);

    return controller;
}

bool mandelbrot_controller_on_event(s_mandelbrot_controller* controller, SDL_Event* event) {
    if(event->type == SDL_MOUSEWHEEL) {
        s_game* game = controller->super.game;

        double old_zoom = controller->zoom;
        double delta_zoom = (double)(1 + (double)event->wheel.y / 30);
        if(delta_zoom < 0.1) delta_zoom = 0.1;
        double zoom = controller->zoom * delta_zoom;

        if(zoom > 1.0) {
            game_set_controller(game, controller->switch_to);
        } else {
            s_proc_vm* videomemory = game->videomemory;
            controller->zoom = zoom;

            double half_width = (double)videomemory->window_width / 2;
            double half_height = (double)videomemory->window_height / 2;

            double center_x = half_width * videomemory->scale;
            double center_y = half_height * videomemory->scale;

            double relative_x = (center_x - (double)controller->old_mouse_x) / videomemory->window_width;
            double relative_y = ((double)controller->old_mouse_y - center_y) / videomemory->window_height;

            controller->camera_x += (videomemory -> window_width * zoom - videomemory->window_width * old_zoom) * relative_x;
            controller->camera_y += (videomemory -> window_height * zoom - videomemory->window_height * old_zoom) * relative_y;

            controller->need_redraw = true;
        }

        return true;
    } else if(event->type == SDL_MOUSEBUTTONDOWN) {
        if(event->button.button == SDL_BUTTON_LEFT) {
            controller->mouse_clicked = true;
            controller->old_mouse_x = event->button.x;
            controller->old_mouse_y = event->button.y;
            return true;
        }
        return false;
    } else if(event->type == SDL_MOUSEBUTTONUP) {
        if(event->button.button == SDL_BUTTON_LEFT) {
            controller->mouse_clicked = false;
            return true;
        }
        return false;
    } else if(event->type == SDL_MOUSEMOTION) {
        if(controller->mouse_clicked) {
            int dx = event->button.x - controller->old_mouse_x;
            int dy = event->button.y - controller->old_mouse_y;

            controller->camera_x -= (double)dx * controller->zoom;
            controller->camera_y += (double)dy * controller->zoom;

            controller->need_redraw = true;
        }

        controller->old_mouse_x = event->button.x;
        controller->old_mouse_y = event->button.y;

        return true;
    } else if(event->type == SDL_KEYDOWN) {
        if(event->key.keysym.sym == SDLK_TAB) {
            controller->function_index++;
            controller->function_index %= 4;
            mandelbrot_controller_update_function(controller);
        } else if(event->key.keysym.sym == SDLK_b) {
            mandelbrot_controller_benchmark(controller);
        } else if(event->key.keysym.sym == SDLK_q) {
            controller->steps--;
        } else if(event->key.keysym.sym == SDLK_e) {
            controller->steps++;
        }

        controller->need_redraw = true;
    }

    return false;
}

void mandelbrot_controller_benchmark(s_mandelbrot_controller* controller) {
    s_game* game = controller->super.game;
    if(!game->videomemory->framebuffer) game_open_window(game);

    int old_index = controller->function_index;

    int cycles = 100;

    printf("Benchmarking %d frames:\n", cycles);

    for(int i = 0; i < 4; i++) {
        controller->function_index = i;
        mandelbrot_controller_update_function(controller);

        clock_t start = clock();

        for(int j = 0; j < cycles; j++) {
            mandelbrot_cluster_perform(controller->cluster, 1);
        }

        clock_t end = clock();
        double time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Estimated FPS:        %f\n", 1.0 / (time / cycles));
    }

    controller->function_index = old_index;
    mandelbrot_controller_update_function(controller);
    controller->need_redraw = true;
}

void mandelbrot_controller_update_function(s_mandelbrot_controller* controller) {
    switch(controller->function_index) {
        case 0:
            printf("Switched to packed doubles\n");
            controller->draw_function = mandelbrot_controller_on_draw_pd_128;
            break;
        case 1:
            printf("Switched to packed floats\n");
            controller->draw_function = mandelbrot_controller_on_draw_ps_128;
            break;
        case 2:
            printf("Switched to no sse\n");
            controller->draw_function = mandelbrot_controller_on_draw_nosse;
            break;
        case 3:
            printf("Switched to messed packed doubles\n");
            controller->draw_function = mandelbrot_controller_on_draw_pd_128_messed;
            break;
    }
}

void mandelbrot_controller_on_enable(s_mandelbrot_controller* controller) {
    s_game* game = controller->super.game;

    game->videomemory->allow_highres = true;
    game_set_size(game, game->width, game->height);

    s_proc_vm* vm = game->videomemory;

    controller->saved_zoom = vm->zoom;
    controller->saved_camera_x = vm->camera_x;
    controller->saved_camera_y = vm->camera_y;

    vm->zoom = 0.5f;
    vm->camera_x = 0.5f;
    vm->camera_y = 0.5f;

    SDL_GetMouseState(&controller->old_mouse_x, &controller->old_mouse_y);
    proc_vm_set_allow_resize(game->videomemory, true);
}

void mandelbrot_controller_on_disable(s_mandelbrot_controller* controller) {
    s_game* game = controller->super.game;
    s_proc_vm* vm = game->videomemory;

    vm->zoom = controller->saved_zoom;
    vm->camera_x = controller->saved_camera_x;
    vm->camera_y = controller->saved_camera_y;
}

void mandelbrot_controller_destroy(s_mandelbrot_controller* controller) {

}

void mandelbrot_controller_release(s_mandelbrot_controller* controller) {
    mandelbrot_controller_destroy(controller);
    free(controller);
}

void mandelbrot_controller_on_tick(s_mandelbrot_controller* controller) {

}

void mandelbrot_controller_on_draw(s_mandelbrot_controller* controller) {
    if(!controller->need_redraw) return;
    controller->need_redraw = false;

    mandelbrot_cluster_perform(controller->cluster, -1);

    proc_vm_redraw(controller->super.game->videomemory);

    SDL_WaitEvent(NULL);
}