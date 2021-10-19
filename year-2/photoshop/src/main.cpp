
#include <SFML/Graphics/RenderWindow.hpp>
#include "ui/ui_screen.hpp"
#include "controls/user_controller.hpp"
#include "test_views/stack_test_view.hpp"
#include "test_views/dropping_menu_test.hpp"
#include "app/photoshop_view.hpp"

int main() {
    const int window_width = 1520;
    const int window_height = 1220;

    sf::RenderWindow window(sf::VideoMode(window_width, window_height, 32), "Photoshop",
                            sf::Style::Titlebar | sf::Style::Close);
    sf::Event event = {};

    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    auto screen = new UIScreen({0, 0}, {window_width, window_height});
//    auto root_view = new DroppingMenuTestView({}, screen->get_size());
//    auto root_view = new LayoutTestView({}, screen->get_size());
    auto root_view = new PhotoshopView({0, 0}, {window_width, window_height});
    UserController controller(&window, screen);
    UIDrawingContext ctx(&window);

    screen->get_view_container()->append_child(root_view);

    while(window.isOpen()) {

        while (window.pollEvent(event)) controller.handle_event(event);

        if(screen->get_needs_redraw()) {
            window.clear();

            screen->prepare_to_draw(&ctx);
            window.display();
            controller.tick();
        } else {
            if(window.waitEvent(event)) controller.handle_event(event);
        }
    }

    delete screen;

    return 0;
}
