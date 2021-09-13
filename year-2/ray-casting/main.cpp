
#include "world.hpp"
#include "camera.hpp"
#include "user_controller.hpp"
#include "renderer.hpp"
#include <SFML/Graphics.hpp>
#include "user_viewport.hpp"
#include "ui/ui_view.hpp"
#include "ui/ui_button.hpp"

const char* BUTTON_TITLES[] = {
    "BUTTON1", "BUTTON2", "BUTTON3"
};

int main() {

    const unsigned int window_width = 500;
    const unsigned int window_height = 200;

    sf::RenderWindow window(sf::VideoMode(window_width, window_height, 32), "Raycaster", sf::Style::Titlebar | sf::Style::Close);
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
    UIView interface({0, 0}, {window_width, window_height});
    UserController controller(&camera, &window, &interface);
    UserViewport viewport { window_width, window_height, pixels, &camera, &world };
    Renderer renderer(&UserViewport::draw, 16, &viewport);
    DrawingContext ctx(&window);

    /* Setting up interface */

    interface.set_background({0, 0, 0, 0});

    for(int i = 1; i <= 3; i++) {
        auto button = new UIButton({window_width - (double)(110 * i), window_height - 70}, {90, 50});
        button->set_title(BUTTON_TITLES[i - 1]);
        interface.append_child(button);
    }

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

        while (window.pollEvent(event)) controller.handle_event(event);

        timer += 0.03;

        sphere2.center = { 5 + sin(timer) * 4, cos(timer) * 4, 0};
        light3.position = sphere2.center;

        window.clear();

        renderer.perform();

        texture.update(pixels);
        window.draw(sprite);
        interface.draw(&ctx);
        window.display();
        controller.tick();
    }

    return 0;
}