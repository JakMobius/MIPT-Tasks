
#include <SFML/Graphics/RenderWindow.hpp>
#include "app/app.hpp"
#include "app/assets.hpp"

int main() {

    const int window_width = 1520;
    const int window_height = 1220;

    sf::RenderWindow window (sf::VideoMode(window_width, window_height, 32), "Photoshop",
                                  sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    Assets.load();
    App app { &window };

    std::function<void(void)> event_processing_loop = [&]() {
        if(!app.opened()) return;
        app.process_events();
        DispatchQueue::main.push({event_processing_loop, 16});
    };

    DispatchQueue::main.push(event_processing_loop);
    DispatchQueue::main.perform();

    Assets.unload();

    return 0;
}
