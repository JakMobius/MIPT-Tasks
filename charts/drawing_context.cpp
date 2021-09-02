//
// Created by Артем on 01.09.2021.
//

#include "drawing_context.hpp"

DrawingContext::DrawingContext(sf::RenderWindow *window): transform(), window(window), font(), vertex_buffer() {
    font.loadFromFile("font.ttf");
    vertex_buffer.construct(32);
}

DrawingContext::~DrawingContext() {
    vertex_buffer.destruct();
}

DrawingContext::DrawingContext(DrawingContext &&other) noexcept:
    transform(std::move(other.transform)),
    window(other.window),
    font(other.font),
    vertex_buffer() {
    vertex_buffer.construct(&other.vertex_buffer);
}

DrawingContext &DrawingContext::operator=(DrawingContext &&other) noexcept {
    if(&other == this) return *this;
    this->transform = other.transform;
    return *this;
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

void DrawingContext::draw_circle(Vec2 center, float radius) {
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

void DrawingContext::draw_text(Vec2 position, char* text) const {
    position.transform(this->transform);

    sf::Text sfText;
    sfText.setCharacterSize(15);
    sfText.setString(text);
    sfText.setFillColor(color);
    sfText.setFont(font);

    if(hAlignment != HTextAlignment::right) {
        float width = sfText.getLocalBounds().width;

        if(hAlignment == HTextAlignment::center) position.x -= width / 2;
        else position.x -= width;
    }

    if(vAlignment != VTextAlignment::bottom) {
        float height = sfText.getLocalBounds().height;

        if(vAlignment == VTextAlignment::center) position.y -= height / 2;
        else position.y -= height;
    }

    sfText.setPosition(position.to_sf_vector());

    window->draw(sfText);
}

void DrawingContext::draw_rect(Vec2 position, Vec2 size) const {

    Vec2 tlVertex = Vec2(position).transform(transform);
    Vec2 trVertex = Vec2(position.x + size.x, position.y).transform(transform);
    Vec2 blVertex = Vec2(position.x, position.y + size.y).transform(transform);
    Vec2 brVertex = Vec2(position.x + size.x, position.y + size.y).transform(transform);

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
