//
// Created by Артем on 20.10.2021.
//

#include "app.hpp"

App::App(sf::RenderWindow* window) : window(window) {

    auto window_size = window->getSize();

    ctx = new UIDrawingContext();
    screen = new UIScreen({0, 0}, {(float)window_size.x, (float)window_size.y});
    render_target = new DrawingTargetWindow(window);
    ctx->push_render_target(render_target);
    controller = new UserController(window, screen);

    auto root_view = new PhotoshopView({0, 0}, {(float)window_size.x, (float)window_size.y});
    screen->get_view_container()->append_child(root_view);

    tick_delayed();
}

void App::tick() {
    ctx->set_stroke_color({0, 0, 0, 1});
    ctx->clear();

    screen->prepare_to_draw(ctx);
    window->display();
    controller->tick();

    if(screen->get_needs_redraw()) {
        tick_delayed();
    }
}

void App::tick_delayed() {
    DispatchQueue::main.push([this] { this->tick(); });
}

App::~App() {
    delete ctx;
    delete screen;
    delete controller;
    delete render_target;
}
