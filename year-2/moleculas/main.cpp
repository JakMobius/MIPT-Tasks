
#include <SFML/Graphics.hpp>
#include "world.hpp"
#include "user_controller.hpp"
#include "creatures/circle.hpp"
#include "creatures/plane.hpp"

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

    World world;
    UserController controller(&window);
    DrawingContext ctx(&window);

    auto* plane1 = new Plane({1000, 10}, PlaneOrientation::vertical);
    auto* plane2 = new Plane({10, 10}, PlaneOrientation::vertical);
    auto* plane3 = new Plane({10, 10}, PlaneOrientation::horizontal);
    auto* plane4 = new Plane({10, 1000}, PlaneOrientation::horizontal);

    auto* circle1 = new Circle({20, 500}, 20);
    auto* circle2 = new Circle({500, 100}, 20);
    auto* hard_circle = new Circle({700, 700}, 100, 1000);

    for(int i = 0; i < 6; i++) {
        world.insert_creature(new Circle({200 + 40 *(double)i, 500}, 20));
        world.insert_creature(new Circle({600 + 40 *(double)i, 500}, 20));

        world.insert_creature(new Circle({500, 200 + 40 *(double)i}, 20));
        world.insert_creature(new Circle({500, 600 + 40 *(double)i}, 20));
    }

    world.insert_creature(plane1);
    world.insert_creature(plane2);
    world.insert_creature(plane3);
    world.insert_creature(plane4);
    world.insert_creature(circle1);
    world.insert_creature(circle2);
    world.insert_creature(hard_circle);

    circle1->get_fixture()->set_velocity({200, 0});
    circle2->get_fixture()->set_velocity({0, 200});
    hard_circle->get_fixture()->set_velocity({200, 200});

    while(window.isOpen()) {

        while (window.pollEvent(event)) controller.handle_event(event);
        window.clear();

        world.draw(&ctx);
        world.tick(1.0 / 60.0, 1);
        window.display();
        controller.tick();

        printf("%c[2K\rTotal energy: %lf j", 27, world.get_total_kynetic_energy());
    }

    return 0;
}
