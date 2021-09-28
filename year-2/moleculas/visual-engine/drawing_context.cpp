//
// Created by Артем on 01.09.2021.
//

#include "drawing_context.hpp"

DrawingContext::DrawingContext(sf::RenderWindow *window): transform(), window(window), font(), vertex_buffer(32) {
    font.loadFromFile("font.ttf");
    h_alignment = HTextAlignmentLeft;
    v_alignment = VTextAlignmentBottom;
    color = sf::Color(255, 255, 255, 255);
}

DrawingContext::~DrawingContext() {

}

void DrawingContext::draw_line(Vec2d from, Vec2d to) const {
    from.transform(transform);
    to.transform(transform);

    sf::Vertex line[] = {
        sf::Vertex(from.to_sf_vector(), color),
        sf::Vertex(to.to_sf_vector(), color)
    };

    window->draw(line, 2, sf::Lines);
}

void DrawingContext::draw_circle(const Vec2d& center, float radius) {
    vertex_buffer.clear();

    const int steps = 30;

    const float angle_step = M_PI * 2 / (float) steps;

    for(int i = 0; i < steps; i++) {
        float angle = angle_step * (float) i;
        Vec2d position = center;
        position.x += sin(angle) * radius;
        position.y += cos(angle) * radius;
        position.transform(this->transform);
        vertex_buffer.push(sf::Vertex(position.to_sf_vector(), color));
    }

    window->draw(vertex_buffer.array, vertex_buffer.size, sf::TriangleFan);
}

void DrawingContext::draw_text(Vec2d position, const char* text) const {
    position.transform(this->transform);

    sf::Text sfText;
    sfText.setCharacterSize(15);
    sfText.setString(text);
    sfText.setFillColor(color);
    sfText.setFont(font);

    if(h_alignment != HTextAlignmentRight) {
        float width = sfText.getLocalBounds().width;

        if(h_alignment == HTextAlignmentCenter) position.x -= width / 2;
        else position.x -= width;
    }

    if(v_alignment != VTextAlignmentBottom) {
        float height = sfText.getLocalBounds().height;

        if(v_alignment == VTextAlignmentCenter) position.y -= height / 2;
        else position.y -= height;
    }

    sfText.setPosition(position.to_sf_vector());

    window->draw(sfText);
}

void DrawingContext::draw_rect(const Vec2d& position, const Vec2d& size) const {

    Vec2d tlVertex(position);
    Vec2d trVertex(position.x + size.x, position.y);
    Vec2d blVertex(position.x, position.y + size.y);
    Vec2d brVertex(position.x + size.x, position.y + size.y);

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

void DrawingContext::set_color(double r, double g, double b, double a) {
    color = sf::Color((sf::Uint8)(r * 255), (sf::Uint8)(g * 255), (sf::Uint8)(b * 255), (sf::Uint8)(a * 255));
}
