//
// Created by Артем on 01.09.2021.
//

#include "drawing_context.hpp"

DrawingContext::DrawingContext() {
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

    target->get_target()->draw(line, 4, sf::TriangleFan);
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

    target->get_target()->draw(&vertex_buffer[0], vertex_buffer.size(), sf::TriangleFan);
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

    target->get_target()->draw(sfText);
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

    target->get_target()->draw(quad, 6, sf::Triangles);
}

DrawingTarget* DrawingContext::get_render_target() {
    return target;
}

void DrawingContext::pop_render_target() {
    target = target_stack[target_stack.size() - 1];
    target_stack.pop_back();
}

void DrawingContext::push_render_target(DrawingTarget* p_target) {
    target = p_target;
    target_stack.push_back(p_target);
}

void DrawingContext::draw_texture(Vec2f position, Vec2f size, DrawingTargetTexture* texture) {
    position *= transform;
    size.transform_unbound(transform);

    sf::Sprite sprite;
    sf::Texture sf_texture = texture->get_texture()->getTexture();
    sprite.setTexture(sf_texture);
    sprite.setPosition(position[0], position[1]);
    auto texture_size = sf_texture.getSize();
    sprite.setScale(size[0] / (float)texture_size.x, size[1] / (float)texture_size.y);
    target->get_target()->draw(sprite);
}

void DrawingContext::clear() {
    target->get_target()->clear(color);
}
