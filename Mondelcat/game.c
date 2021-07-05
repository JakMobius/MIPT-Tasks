
#include "game.h"

bool game_set_size(s_game* game, int width, int height) {
    if(!game->videomemory->has_window) {
        game->width = width;
        game->height = height;

        return true;
    }

    if(!proc_vm_set_size(game->videomemory, width, height)) return false;

    game->width = width;
    game->height = height;

    return true;
}

s_texture* texture_clone(s_texture* texture) {
    if(!texture) return NULL;

    char* new_buffer = calloc(buffer_size(texture->texture_info.biWidth, texture->texture_info.biHeight), sizeof(char));
    if(!new_buffer) return NULL;
    memcpy(new_buffer, texture->texture, texture->texture_info.biSizeImage);

    s_texture* new_texture = game_texture_create(new_buffer, texture->texture_info, texture->has_alpha);
    if(!new_texture) {
        free(new_buffer);
        return NULL;
    }

    return new_texture;
}

s_texture* load_texture(char* filename, bool has_alpha) {
    s_bitmap_info_header texture_info = {0};
    char* table_bmp = load_bitmap_file(filename, &texture_info);
    if(!table_bmp) return NULL;

    s_texture* texture = game_texture_create(table_bmp, texture_info, has_alpha);
    if(!texture) {
        free(table_bmp);
        return NULL;
    }

    return texture;
}

s_sprite* load_sprite(char* filename, bool has_alpha, int x, int y) {
    s_texture* texture = load_texture(filename, has_alpha);
    if(!texture) {
        return NULL;
    }

    s_sprite* sprite = game_sprite_create(x, y, texture);
    if(!sprite) {
        game_texture_release(texture);
        return NULL;
    }

    return sprite;
}

s_game* game_create() {
    s_game* game = calloc(1, sizeof(s_game));

    game->videomemory = proc_vm_create();

    if(!game->videomemory) {
        game_release(game);
        return NULL;
    }

    init_blend_constants();

    return game;
}

void game_set_controller(s_game* game, s_game_controller* game_controller) {
    if(game->controller) {
        game->controller->on_disable(game->controller);
        game->controller->game = NULL;
    }

    game->controller = game_controller;

    if(game_controller) {
        game_controller->game = game;
        game->controller->on_enable(game_controller);
    }
}

void game_release(s_game* game) {
    game_destroy(game);
    free(game);
}

void game_destroy(s_game* game) {
    if(game->videomemory) {
        proc_vm_destroy(game->videomemory);
    }
}

s_texture* game_texture_create(char* buffer, s_bitmap_info_header info, bool has_alpha) {
    s_texture* texture = calloc(1, sizeof(s_texture));
    if(!texture) return NULL;

    texture->texture = buffer;
    texture->has_alpha = has_alpha;
    texture->texture_info = info;

    return texture;
}

s_sprite* game_sprite_create(int x, int y, s_texture* texture) {
    s_sprite* sprite = calloc(1, sizeof(s_sprite));
    if(!sprite) return NULL;

    sprite->x = x;
    sprite->y = y;
    sprite->texture = texture;

    return sprite;
}

void game_sprite_release(s_sprite* sprite) {
    game_sprite_destroy(sprite);
    free(sprite);
}

void game_sprite_destroy(s_sprite* sprite) {
    if(sprite->texture) game_texture_release(sprite->texture);
}

void game_texture_release(s_texture* texture) {
    game_texture_destroy(texture);
    free(texture);
}

void game_texture_destroy(s_texture* texture) {
    if(texture->texture) free(texture->texture);
}

bool game_open_window(s_game* game) {
    return proc_vm_init_window(game->videomemory, game->width, game->height);
}

void game_default_event_handler(s_game* game, SDL_Event* event) {
    if(event->type == SDL_QUIT) game->alive = false;
    if(event->type == SDL_WINDOWEVENT) {
        if(event->window.event == SDL_WINDOWEVENT_RESIZED) {
            int width = event->window.data1;
            int height = event->window.data2;

            proc_vm_update_virtual_size(game->videomemory, width, height);
        }
    }
}

void game_loop_until_alive(s_game* game) {
    if(!game->videomemory->framebuffer) game_open_window(game);
    game->alive = true;

    SDL_Event event = {0};

    while(game->alive) {
        while(SDL_PollEvent(&event)) {
            if(!game->controller || !game->controller->on_event(game->controller, &event)) {
                game_default_event_handler(game, &event);
            }
        }
        if(game->controller) {
            game->controller->on_tick(game->controller);
            game->controller->on_draw(game->controller);
        }
        if(event.type == SDL_QUIT) game->alive = false;
    }
}

void game_draw_sprite(s_game* game, s_sprite* sprite) {

    s_texture* texture = sprite->texture;

    if(texture->has_alpha) {
        blend_bmp_to_vm(game->videomemory, texture->texture, &texture->texture_info, sprite->x, sprite->y);
    } else {
        write_bmp_to_vm(game->videomemory, texture->texture, &texture->texture_info, sprite->x, sprite->y);
    }
}
