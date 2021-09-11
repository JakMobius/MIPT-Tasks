//
// Created by Артем on 01.09.2021.
//

#include "drawing_context.hpp"

DrawingContext::DrawingContext(sf::RenderWindow *window): transform(), window(window), font(), vertex_buffer(32) {
    font.loadFromFile("font.ttf");
    hAlignment = HTextAlignmentLeft;
    vAlignment = VTextAlignmentBottom;
    color = sf::Color(255, 255, 255, 255);
}

DrawingContext::~DrawingContext() {

}

void DrawingContext::draw_line(Vec2 from, Vec2 to) const {
    from.transform(transform);
    to.transform(transform);

    sf::Vertex line[] = {
        sf::Vertex(from.to_sf_vector(), color),
        sf::Vertex(to.to_sf_vector(), color)
    };

    window->draw(line, 2, sf::Lines);
}

void DrawingContext::draw_circle(const Vec2& center, float radius) {
    vertex_buffer.clear();

    const int steps = 30;

    const float angle_step = M_PI * 2 / (float) steps;

    for(int i = 0; i < steps; i++) {
        float angle = angle_step * (float) i;
        Vec2 position = center;
        position.x += sin(angle) * radius;
        position.y += cos(angle) * radius;
        position.transform(this->transform);
        vertex_buffer.push(sf::Vertex(position.to_sf_vector(), color));
    }

    window->draw(vertex_buffer.array, vertex_buffer.size, sf::TriangleFan);
}

void DrawingContext::draw_text(Vec2 position, const char* text) const {
    position.transform(this->transform);

    sf::Text sfText;
    sfText.setCharacterSize(15);
    sfText.setString(text);
    sfText.setFillColor(color);
    sfText.setFont(font);

    if(hAlignment != HTextAlignmentRight) {
        float width = sfText.getLocalBounds().width;

        if(hAlignment == HTextAlignmentCenter) position.x -= width / 2;
        else position.x -= width;
    }

    if(vAlignment != VTextAlignmentBottom) {
        float height = sfText.getLocalBounds().height;

        if(vAlignment == VTextAlignmentCenter) position.y -= height / 2;
        else position.y -= height;
    }

    sfText.setPosition(position.to_sf_vector());

    window->draw(sfText);
}

void DrawingContext::draw_rect(const Vec2& position, const Vec2& size) const {

    Vec2 tlVertex(position);
    Vec2 trVertex(position.x + size.x, position.y);
    Vec2 blVertex(position.x, position.y + size.y);
    Vec2 brVertex(position.x + size.x, position.y + size.y);

    tlVertex.transform(transform);
    trVertex.transform(transform);
    blVertex.transform(transform);
    brVertex.transform(transform);

    sf::Vertex quad[] = {
        sf::Vertex(tlVertex.to_sf_vector(), color),
        sf::Vertex(trVertex.to_sf_vector(), color),
        sf::Vertex(blVertex.to_sf_vector(), color),
        sf::Vertex(trVertex.to_sf_vector(), color),
        sf::Vertex(blVertex.to_sf_vector(), color),
        sf::Vertex(brVertex.to_sf_vector(), color)
    };

    window->draw(quad, 6, sf::Triangles);
}