
#ifndef MANDELBROT_CAT_CONTROLLER_H
#define MANDELBROT_CAT_CONTROLLER_H

#include "game.h"

typedef struct cat_sprites {
    s_sprite* table_sprite;
    s_sprite* sleeping_cat;
    s_sprite* left_cat;
    s_sprite* right_cat;
    s_sprite* flying_cat;
} s_cat_sprites;

typedef struct cat_controller {
    s_game_controller super;
    s_cat_sprites sprites;

    s_game_controller* switch_to;
} s_cat_controller;

s_cat_controller* cat_controller_create();
void cat_controller_destroy(s_cat_controller* controller);
void cat_controller_release(s_cat_controller* controller);
bool cat_controller_load_sprites(s_cat_controller* controller);
void cat_controller_on_enable(s_cat_controller* controller);
void cat_controller_on_disable(s_cat_controller* controller);
bool cat_controller_on_event(s_cat_controller* controller, SDL_Event* event);
void cat_controller_on_tick(s_cat_controller* controller);
void cat_controller_on_draw(s_cat_controller* controller);
void cat_controller_benchmark(s_cat_controller* controller);

#endif //MANDELBROT_CAT_CONTROLLER_H
