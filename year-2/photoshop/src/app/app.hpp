#pragma once

#include "../ui/ui_screen.hpp"
#include "../controls/user_controller.hpp"
#include "../graphics/drawing_target_window.hpp"
#include "../utils/dispatch_queue.hpp"

class App {
    DrawingContext* ctx = nullptr;
    sf::RenderWindow* window = nullptr;
    UIScreen* screen = nullptr;
    UserController* controller = nullptr;
    DrawingTargetWindow* render_target = nullptr;

public:
    explicit App(sf::RenderWindow* window);
    ~App();

    void render_delayed();
    void render();

    UserController* get_controller() const { return controller; }

    void process_events();

    bool opened();
};
