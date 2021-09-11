#include <SFML/Graphics.hpp>
#include "drawing_context.hpp"
#include "graph.hpp"
#include "awesome_functions.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 1200), "Grapher");
    DrawingContext ctx(&window);

    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    Graph graph1(&parabola,    Vec2(10, 10),   Vec2(320, 400));
    Graph graph2(&taylor_sine, Vec2(450, 130), Vec2(350, 300));
    Graph graph3(&tan,         Vec2(100, 500), Vec2(500, 600));
    Graph graph4(&acos,        Vec2(740, 450), Vec2(700, 600));
    Graph graph5(&float_hash,  Vec2(1200, 20), Vec2(300, 400));

    graph1.set_viewport(Vec2(10, 10),   Vec2(0, 0),   0);
    graph2.set_viewport(Vec2(10, 10),   Vec2(0, 0),   0.1);
    graph3.set_viewport(Vec2(30, 30),   Vec2(0, 0),   -0.3);
    graph4.set_viewport(Vec2(180, 180), Vec2(0, 1.5), -0.6);
    graph5.set_viewport(Vec2(100, 100), Vec2(0, 0),   0.2);

    VisualVector arrow1(Vec2(2, 2),  Vec2(5, 2), VisualVectorStyleArrowed);
    VisualVector arrow2(Vec2(5, 10), Vec2(5, 2), VisualVectorStyleCircled);
    VisualVector arrow3(Vec2(0, -15), Vec2(0, 0), VisualVectorStyleArrowed);

    graph1.add_vector(&arrow1);
    graph1.add_vector(&arrow2);
    graph2.add_vector(&arrow3);

    float animation_timer = 0;

    while (window.isOpen()) {
        sf::Event event = {};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        graph1.draw(&ctx);
        graph2.draw(&ctx);
        graph3.draw(&ctx);
        graph4.draw(&ctx);
        graph5.draw(&ctx);

        animation_timer += 0.05;
        float animation_position = sin(animation_timer) * 10;

        arrow3.projections.x = animation_position - arrow3.base.x;
        arrow3.projections.y = graph2.get_function()(arrow3.projections.x) - arrow3.base.y;

        window.display();
    }

    return 0;
}
