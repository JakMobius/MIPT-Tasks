#pragma once

#include "../ui/ui_screen.hpp"
#include "../controls/user-controller.hpp"
#include "../graphics/drawing-target-window.hpp"
#include "../utils/dispatch_queue.hpp"

class App {
    DrawingContext* ctx = nullptr;
    sf::RenderWindow* window = nullptr;
    UIScreen* screen = nullptr;
    UserController* controller = nullptr;
    DrawingTargetWindow* render_target = nullptr;
    bool render_queried = false;

public:
    explicit App(sf::RenderWindow* window);
    ~App();

    void render_delayed();
    void render();

    UserController* get_controller() const { return controller; }

    void process_events();

    bool opened();
};
