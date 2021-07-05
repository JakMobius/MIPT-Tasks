#include <stdio.h>
#include "videomemory.h"
#include "color_blend.h"
#include "game.h"
#include "cat_controller.h"
#include "mandelbrot_controller.h"
#include "messed_drawer.h"

int main() {

    s_game* game = game_create();

    if(!game) {
        printf("Could not create game\n");
        return 1;
    }

    s_cat_controller* cat_controller = cat_controller_create();
    s_mandelbrot_controller* mandelbrot_controller = mandelbrot_controller_create();

    if(!cat_controller || !mandelbrot_controller) {
        printf("Could not create controllers\n");
        game_destroy(game);
        return 1;
    }

    cat_controller->switch_to = (s_game_controller*) mandelbrot_controller;
    mandelbrot_controller->switch_to = (s_game_controller*) cat_controller;

    game_set_controller(game, (s_game_controller*) cat_controller);

    if(!game_open_window(game)) {
        printf("Could not present window\n");
        game_destroy(game);
        cat_controller_release(cat_controller);
        mandelbrot_controller_release(mandelbrot_controller);
        return 1;
    }

//    cat_controller_benchmark(cat_controller);
//    game_set_controller(game, NULL);
    game_loop_until_alive(game);

    game_destroy(game);

    return 0;
}