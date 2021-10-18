//
// Created by Артем on 01.09.2021.
//

#include "drawing_context.hpp"

DrawingContext::DrawingContext(sf::RenderWindow *window): vertex_buffer(32), transform(), window(window), font() {
    font.loadFromFile("font.ttf");
    hAlignment = HTextAlignmentLeft;
    vAlignment = VTextAlignmentBottom;
    color = sf::Color(255, 255, 255, 255);
}

DrawingContext::~DrawingContext() {

}

void DrawingContext::draw_line(Vec2f from, Vec2f to, float thickness) const {

    Vec2f shape[] = {
        from, from, to, to
    };


    Vec2f ortho_offset = { to[1] - from[1], - to[0] + from[0] };
    ortho_offset.normalize();
    ortho_offset *= thickness / 2;

    (shape[0] -= ortho_offset) *= transform;
    (shape[1] += ortho_offset) *= transform;
    (shape[2] += ortho_offset) *= transform;
    (shape[3] -= ortho_offset) *= transform;

    sf::Vertex line[] = {
        sf::Vertex({shape[0][0], shape[0][1]}, color),
        sf::Vertex({shape[1][0], shape[1][1]}, color),
        sf::Vertex({shape[2][0], shape[2][1]}, color),
        sf::Vertex({shape[3][0], shape[3][1]}, color)
    };

    window->draw(line, 4, sf::TriangleFan);
}

void DrawingContext::draw_circle(const Vec2f& center, float radius) {
    vertex_buffer.clear();

    const int steps = 30;

    const float angle_step = M_PI * 2 / (float) steps;

    for(int i = 0; i < steps; i++) {
        float angle = angle_step * (float) i;
        Vec2f position = center;
        position.set_x(position[0] + sin(angle) * radius);
        position.set_y(position[1] + cos(angle) * radius);
        position *= this->transform;
        vertex_buffer.push_back(sf::Vertex({position[0], position[1]}, color));
    }

    window->draw(&vertex_buffer[0], vertex_buffer.size(), sf::TriangleFan);
}

void DrawingContext::draw_text(Vec2f position, const char* text) const {
    position *= this->transform;

    sf::Text sfText;
    sfText.setCharacterSize(15);
    sfText.setString(text);
    sfText.setFillColor(color);
    sfText.setFont(font);

    if(hAlignment != HTextAlignmentRight) {
        float width = sfText.getLocalBounds().width;

        if(hAlignment == HTextAlignmentCenter) position.set_x(position[0] - width / 2);
        else position.set_x(position[0] - width);
    }

    if(vAlignment != VTextAlignmentBottom) {
        float height = sfText.getLocalBounds().height;

        if(vAlignment == VTextAlignmentCenter) position.set_y(position[1] - height / 2);
        else position.set_y(position[1] - height);
    }

    sfText.setPosition({position[0], position[1]});

    window->draw(sfText);
}

void DrawingContext::draw_rect(const Vec2f& position, const Vec2f& size) const {

    Vec2f tlVertex(position);
    Vec2f trVertex(position[0] + size[0], position[1]);
    Vec2f blVertex(position[0], position[1] + size[1]);
    Vec2f brVertex(position[0] + size[0], position[1] + size[1]);

    tlVertex *= transform;
    trVertex *= transform;
    blVertex *= transform;
    brVertex *= transform;

    sf::Vertex quad[] = {
        sf::Vertex({tlVertex[0], tlVertex[1]}, color),
        sf::Vertex({trVertex[0], trVertex[1]}, color),
        sf::Vertex({blVertex[0], blVertex[1]}, color),
        sf::Vertex({trVertex[0], trVertex[1]}, color),
        sf::Vertex({blVertex[0], blVertex[1]}, color),
        sf::Vertex({brVertex[0], brVertex[1]}, color)
    };

    window->draw(quad, 6, sf::Triangles);
}