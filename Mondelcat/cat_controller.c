
#include "cat_controller.h"

static long long min(long long a, long long b) {
    return a < b ? a : b;
}

static long long max(long long a, long long b) {
    return a > b ? a : b;
}

s_cat_controller* cat_controller_create() {
    s_cat_controller* controller = calloc(1, sizeof(s_cat_controller));
    if(!controller) return NULL;

    if(!cat_controller_load_sprites(controller)) {
        cat_controller_release(controller);
    }

    controller->super.on_draw = (void (*)(s_game_controller*)) cat_controller_on_draw;
    controller->super.on_tick = (void (*)(s_game_controller*)) cat_controller_on_tick;
    controller->super.on_enable = (void (*)(s_game_controller*)) cat_controller_on_enable;
    controller->super.on_disable = (void (*)(s_game_controller*)) cat_controller_on_disable;
    controller->super.on_event = (bool (*)(s_game_controller*, SDL_Event* event)) cat_controller_on_event;

    return controller;
}

bool cat_controller_on_event(s_cat_controller* controller, SDL_Event* event) {
    if(event->type == SDL_MOUSEWHEEL) {
        float zoom = controller->super.game->videomemory->zoom * (float)(1 + (float)event->wheel.y / 20);

        s_proc_vm* vm = controller->super.game->videomemory;

        if(zoom > 0.5f) zoom = 0.5f;
        vm->zoom = zoom;
        vm->camera_x = 0.5f * (zoom * 2) + 0.593f * (1 - zoom * 2);
        vm->camera_y = 0.5f * (zoom * 2) + 0.636f * (1 - zoom * 2);

        if(zoom < 0.0003f) {
            game_set_controller(controller->super.game, controller->switch_to);
        }

        return true;
    }

    return false;
}

void cat_controller_on_enable(s_cat_controller* controller) {
    s_game* game = controller->super.game;
    game->videomemory->allow_highres = false;

    s_bitmap_info_header* texture_info = &controller->sprites.table_sprite->texture->texture_info;

    if(!game_set_size(game, texture_info->biWidth, texture_info->biHeight)) {
        printf("Could not update window size\n");
        game_set_controller(game, NULL);
    }

    proc_vm_set_allow_resize(game->videomemory, false);
}

void cat_controller_on_disable(s_cat_controller* controller) {

}

void cat_controller_destroy(s_cat_controller* controller) {
    s_sprite** game_sprites = (s_sprite**) &(controller->sprites);

    for(int i = 0; i < sizeof(s_cat_sprites) / sizeof(s_sprite*); i++) {
        if(game_sprites[i]) game_sprite_release(game_sprites[i]);
    }
}

void cat_controller_release(s_cat_controller* controller) {
    cat_controller_destroy(controller);
    free(controller);
}

void cat_controller_on_tick(s_cat_controller* controller) {
    s_game* game = controller->super.game;
    s_cat_sprites* sprites = &controller->sprites;
    game->ticks++;

    long long amplitude = 400;
    long long speed = 5;
    long long ball_position = (game->ticks * speed) % (amplitude * 2);
    if(ball_position > amplitude) ball_position = amplitude * 2 - ball_position;
    ball_position -= amplitude / 2;

    if(game->ticks * speed % amplitude == 0) {
        bmp_flip_image_horizontal(sprites->flying_cat->texture->texture, &sprites->flying_cat->texture->texture_info);
    }

    long long amplitude_threshold = amplitude * 3 / 4;

    long long left_jump = amplitude / 4 - (max(amplitude_threshold, -ball_position + amplitude / 2) - amplitude_threshold);
    long long right_jump = amplitude / 4 - (max(amplitude_threshold, ball_position + amplitude / 2) - amplitude_threshold);

    left_jump = left_jump * left_jump / amplitude_threshold;
    right_jump = right_jump * right_jump / amplitude_threshold;

    sprites->flying_cat->x = (int)(game->width / 2 + ball_position - sprites->flying_cat->texture->texture_info.biWidth / 2);
    sprites->flying_cat->y = (int)(500 - ball_position * ball_position * 300 / amplitude / amplitude);
    sprites->left_cat->y = 240 - left_jump;
    sprites->right_cat->y = 240 - right_jump;
}

void cat_controller_on_draw(s_cat_controller* controller) {
    s_game* game = controller->super.game;
    s_cat_sprites* sprites = &controller->sprites;

    game_draw_sprite(game, sprites->table_sprite);
    game_draw_sprite(game, sprites->left_cat);
    game_draw_sprite(game, sprites->right_cat);
    game_draw_sprite(game, sprites->sleeping_cat);
    game_draw_sprite(game, sprites->flying_cat);

    proc_vm_redraw(game->videomemory);
}

bool cat_controller_load_sprites(s_cat_controller* controller) {

    controller->sprites.table_sprite = load_sprite("resources/Table.bmp", false, 0, 0);
    controller->sprites.sleeping_cat = load_sprite("resources/AskhatCat.bmp", true, 300, 280);
    controller->sprites.flying_cat   = load_sprite("resources/cat_flying.bmp", true, 350, 450);

    s_texture* cat_texture_left = load_texture("resources/cat_playing.bmp", true);
    s_texture* cat_texture_right = texture_clone(cat_texture_left);

    if(cat_texture_left && cat_texture_right) {
        bmp_flip_image_horizontal(cat_texture_right->texture, &cat_texture_right->texture_info);

        controller->sprites.left_cat = game_sprite_create(0, 0, cat_texture_left);
        controller->sprites.right_cat = game_sprite_create(0, 0, cat_texture_right);
    } else {
        game_texture_release(cat_texture_left);
        game_texture_release(cat_texture_right);
    }

    if(!controller->sprites.table_sprite ||
       !controller->sprites.sleeping_cat ||
       !controller->sprites.left_cat ||
       !controller->sprites.right_cat ||
       !controller->sprites.flying_cat) {
        return false;
    }

    controller->sprites.left_cat->x = 20;
    controller->sprites.left_cat->y = 300;
    controller->sprites.right_cat->x = 640;
    controller->sprites.right_cat->y = 300;

    return true;
}

void cat_controller_benchmark(s_cat_controller* controller) {
    s_game* game = controller->super.game;
    if(!game->videomemory->framebuffer) game_open_window(game);

    int cycles = 5000;

    printf("Benchmark started\n");

    clock_t start = clock();

    for(int i = 0; i < cycles; i++) {
        game_draw_sprite(game, controller->sprites.table_sprite);
        game_draw_sprite(game, controller->sprites.sleeping_cat);
    }

    clock_t end = clock();
    double time = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Benchmark complete\n");
    printf("Total frames:         %d\n", cycles);
    printf("Total time:           %f seconds\n", time);
    printf("Estimated frame time: %f seconds\n", time / cycles);
    printf("Estimated FPS:        %f\n", 1.0 / (time / cycles));

    proc_vm_redraw(game->videomemory);
}