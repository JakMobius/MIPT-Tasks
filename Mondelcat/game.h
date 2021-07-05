
#ifndef MANDELBROT_GAME_H
#define MANDELBROT_GAME_H

#include "videomemory.h"
#include "bmp_reader.h"
#include "color_blend.h"
//#include "cat_controller.h"

typedef struct game s_game;

typedef struct texture {
    char* texture;
    s_bitmap_info_header texture_info;
    bool has_alpha;
} s_texture;

typedef struct sprite {
    int x;
    int y;
    s_texture* texture;
} s_sprite;

typedef struct game_controller {
    void (* on_tick)(struct game_controller* controller);
    void (* on_draw)(struct game_controller* controller);
    void (* on_enable)(struct game_controller* controller);
    void (* on_disable)(struct game_controller* controller);
    bool (* on_event)(struct game_controller* controller, SDL_Event* event);
    s_game* game;
} s_game_controller;

//typedef struct active_controller {
//    s_game_controller* instance;
//
//    void behave(s_game_controller* prototype) {
//        instance = prototype;
//    }
//
//    void on_tick() {
//        instance->on_tick();
//    }
//
//    void on_draw() {
//        instance->on_draw();
//    }
//
//    void on_enable() {
//        instance->on_enable();
//    }
//
//    void on_disable() {
//        instance->on_disable();
//    }
//
//    bool on_event(SDL_Event* event) {
//        instance->on_event(event);
//    }
//
//} s_active_controller;
//
//void be() {
//    s_cat_controller* cat_controller = cat_controller_create();
//    s_mandelbrot_controller* mandelbrot_controller = mandelbrot_controller_create();
//
//    s_active_controller active_controller; // =
//
//    active_controller.behave(cat_controller);
//}

typedef struct game {
    int width;
    int height;
    s_proc_vm* videomemory;
    bool alive;
    long long ticks;
    s_game_controller* controller;
} s_game;

s_game* game_create();
void game_destroy(s_game* game);
void game_release(s_game* game);
bool game_set_size(s_game* game, int width, int height);
s_sprite* game_sprite_create(int x, int y, s_texture* texture);
void game_sprite_release(s_sprite* sprite);
void game_sprite_destroy(s_sprite* sprite);
s_texture* game_texture_create(char* buffer, s_bitmap_info_header info, bool has_alpha);
void game_texture_release(s_texture* texture);
void game_texture_destroy(s_texture* texture);
void game_loop_until_alive(s_game* game);
void game_benchmark(s_game* game);
void game_tick(s_game* game);
void game_draw_sprite(s_game* game, s_sprite* sprite);
s_texture* load_texture(char* filename, bool has_alpha);
s_sprite* load_sprite(char* filename, bool has_alpha, int x, int y);
s_texture* texture_clone(s_texture* texture);
bool game_open_window(s_game* game);
void game_set_controller(s_game* game, s_game_controller* game_controller);
void game_default_event_handler(s_game* game, SDL_Event* event);

#endif //MANDELBROT_GAME_H
