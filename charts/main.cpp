#include <iostream>
#include <SFML/Graphics.hpp>
#include "drawing_context.hpp"
#include "graph.hpp"

float parabola(float x) {
    return x * x * sin(x * 1);
}

float taylor_sine(float x) {
    int terms = 15;
    double result = x;
    double numerator = x;
    double denominator = 1;

    for(int i = 1; i < terms; i++) {
        numerator *= x * x;
        denominator *= i * (i * 4 + 2);
        double term = numerator / denominator;
        if((i & 1) == 1) term = -term;
        result += term;
    }

    return (float)result;
}

float hash(float a) {
    return (float)(*((int*)(&a)) % 100) / 100;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 1200), "Grapher");
    DrawingContext ctx(&window);

    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    Graph graph1(&parabola,    {10, 10},   {320, 400});
    Graph graph2(&taylor_sine, {450, 130}, {350, 300});
    Graph graph3(&tan,         {100, 500}, {500, 600});
    Graph graph4(&acos,        {740, 450}, {700, 600});
    Graph graph5(&hash,        {1200, 20}, {300, 400});

    graph1.set_viewport({10, 10},   {0, 0},   0);
    graph2.set_viewport({10, 10},   {0, 0},   0.1);
    graph3.set_viewport({30, 30},   {0, 0},   -0.3);
    graph4.set_viewport({180, 180}, {0, 1.5}, -0.6);
    graph5.set_viewport({100, 100}, {0, 0},   0.2);

    graph1.vectors_to_draw.push(VisualVector({2, 2}, {5, 2}, VisualVectorStyle::arrowed));
    graph1.vectors_to_draw.push(VisualVector({5, 10}, {5, 2}, VisualVectorStyle::circled));

    while (window.isOpen()) {
        sf::Event event = {};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        // graph1.graph_angle += 0.05;
        // graph2.graph_angle -= 0.07;

        graph1.draw(&ctx);
        graph2.draw(&ctx);
        graph3.draw(&ctx);
        graph4.draw(&ctx);
        graph5.draw(&ctx);

        window.display();
    }

    return 0;
}
