
#include "world.hpp"
#include "camera.hpp"
#include "user_controller.hpp"
#include "renderer.hpp"
#include <SFML/Graphics.hpp>
#include "user_viewport.hpp"

int main() {

    const unsigned int window_width = 1440;
    const unsigned int window_height = 720;

    sf::RenderWindow window(sf::VideoMode(window_width, window_height, 32), "Raycaster");
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Uint8* pixels  = new sf::Uint8[window_width * window_height * 4];
    sf::Event event = {};

    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    texture.create(window_width, window_height);
    sprite.setTexture(texture);

    World world;
    Camera camera({0, 0, 0});
    UserController controller(&camera, &window);
    UserViewport viewport { window_width, window_height, pixels, &camera, &world };
    Renderer renderer(&UserViewport::draw, 16, &viewport);

    /* Setting up the world */

    Sphere sphere1({5, 0, 0}, 1);
    Sphere sphere2({0, 0, 0}, 0.5);

    LightPoint light1({4, -2, -2}, 0.3);
    LightPoint light2({4, 2, 1}, 0.3, {1, 1, 0});
    LightPoint light3({0, 0, 0}, 0.8, {0, 0, 1});

    sphere1.color = { 1.0, 0.5, 0.0 };
    sphere2.color = { 0.0, 0.5, 1.0 };

    world.add_sphere(&sphere1);
    world.add_sphere(&sphere2);
    world.add_light_point(&light1);
    world.add_light_point(&light2);
    world.add_light_point(&light3);

    float timer = 0;

    while(window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            else controller.handle_event(event);
        }

        timer += 0.03;

        sphere2.center = { 5 + sin(timer) * 4, cos(timer) * 4, 0};
        light3.position = sphere2.center;

        window.clear();

        renderer.perform();

        texture.update(pixels);
        window.draw(sprite);
        window.display();
        controller.tick();
    }

    return 0;
}