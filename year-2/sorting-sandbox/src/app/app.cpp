//
// Created by Артем on 20.10.2021.
//

#include "app.hpp"
#include "app-view.hpp"

App::App(sf::RenderWindow* window) : window(window) {

    auto window_size = window->getSize();

    ctx = new DrawingContext();
    screen = new UIScreen({0, 0}, {(float)window_size.x, (float)window_size.y});
    screen->focus();
    render_target = new DrawingTargetWindow(window);
    ctx->push_render_target(render_target);
    controller = new UserController(window, screen);

    auto root_view = new AppView(this, {0, 0}, {(float)window_size.x, (float)window_size.y});
    screen->get_view_container()->append_child(root_view);
}

void App::render() {
    render_queried = false;
    if(!screen->get_needs_redraw()) return;

    ctx->clear({0, 0, 0, 1});

    screen->prepare_to_draw(ctx);
    window->display();

    if(screen->get_hidden()) {
        render_delayed();
    }
}

void App::render_delayed() {
    if(render_queried) return;
    render_queried = true;
    DispatchQueue::main.push(DispatchQueueTask { [this] { this->render(); }, 1000 / 60 });
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
    if(screen->get_needs_redraw()) render_delayed();
}

bool App::opened() {
    return window->isOpen();
}
