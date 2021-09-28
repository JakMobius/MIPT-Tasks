
#include <SFML/Graphics.hpp>
#include "visual-engine/user_controller.hpp"
#include "visual-engine/drawing_context.hpp"
#include "game/scene.hpp"

int main() {

    const unsigned int window_width = 1500;
    const unsigned int window_height = 1200;

    sf::RenderWindow window(sf::VideoMode(window_width, window_height, 32), "Moleculas",
                            sf::Style::Titlebar | sf::Style::Close);
    sf::Texture texture;
    sf::Event event = {};

    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    texture.create(window_width, window_height);

    DrawingContext ctx(&window);
    GameScene scene({0, 0}, {1500, 1200});
    UserController controller(&window, &scene);

    scene.init_scene();

    while(window.isOpen()) {

        while (window.pollEvent(event)) controller.handle_event(event);
        window.clear();

        scene.draw(&ctx);
        scene.tick(1.0 / 60.0);
        window.display();
        controller.tick();

        printf("%c[2K\rTotal energy: %lf j", 27, scene.get_physics_world()->get_total_kinetic_energy());
    }

    return 0;
}
