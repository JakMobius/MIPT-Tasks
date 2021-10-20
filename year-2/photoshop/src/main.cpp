
#include <SFML/Graphics/RenderWindow.hpp>
#include "app/app.hpp"

int main() {

    const int window_width = 1520;
    const int window_height = 1220;

    sf::RenderWindow window (sf::VideoMode(window_width, window_height, 32), "Photoshop",
                                  sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    App app { &window };

    while(window.isOpen()) {
        sf::Event event = {};

        if(DispatchQueue::main.empty()) {
            if(window.waitEvent(event)) app.get_controller()->handle_event(event);
            app.tick_delayed();
        } else {
            while (window.pollEvent(event)) app.get_controller()->handle_event(event);
        }

        DispatchQueue::main.perform();
    }

    return 0;
}
