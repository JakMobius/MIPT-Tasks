//
// Created by Артем on 20.10.2021.
//

#include "app.hpp"
#include "photoshop_view.hpp"

App::App(sf::RenderWindow* window) : window(window) {

    auto window_size = window->getSize();

    ctx = new DrawingContext();
    screen = new UIScreen({0, 0}, {(float)window_size.x, (float)window_size.y});
    render_target = new DrawingTargetWindow(window);
    ctx->push_render_target(render_target);
    controller = new UserController(window, screen);

    auto root_view = new PhotoshopView(this, {0, 0}, {(float)window_size.x, (float)window_size.y});
    screen->get_view_container()->append_child(root_view);

    tick_delayed();
}

void App::tick() {
    if(!screen->get_needs_redraw()) return;

    ctx->clear({0, 0, 0, 1});

    screen->prepare_to_draw(ctx);
    window->display();

    if(screen->get_needs_redraw()) {
        tick_delayed();
    }
}

void App::tick_delayed() {
    DispatchQueue::main.push(DispatchQueueTask { [this] { this->tick(); } });
}

App::~App() {
    delete ctx;
    delete screen;
    delete controller;
    delete render_target;
}

void App::process_events() {
    sf::Event event {};
    while (window->pollEvent(event)) get_controller()->handle_event(event);
    tick_delayed();
}

bool App::opened() {
    return window->isOpen();
}
