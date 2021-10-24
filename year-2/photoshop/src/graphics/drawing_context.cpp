//
// Created by Артем on 01.09.2021.
//

#include "drawing_context.hpp"
#include "../app/assets.hpp"

DrawingContext::DrawingContext() {
    font = Assets.default_font;
    hAlignment = HTextAlignmentLeft;
    vAlignment = VTextAlignmentBottom;
}

DrawingContext::~DrawingContext() {

}

void DrawingContext::stroke_line(Vec2f from, Vec2f to, float thickness) const {
    if(!stroke_style) return;

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
        stroke_style->vertex(shape[0], -1, 1),
        stroke_style->vertex(shape[1], -1, -1),
        stroke_style->vertex(shape[2], 1, -1),
        stroke_style->vertex(shape[3], 1, 1)
    };

    auto render_states = stroke_style->get_render_states();
    if(render_states) target->get_target()->draw(line, 4, sf::TriangleFan, *render_states);
    else target->get_target()->draw(line, 4, sf::TriangleFan);
}

void DrawingContext::fill_circle(const Vec2f& center, float radius) {
    if(!fill_style) return;
    vertex_buffer.clear();

    const int steps = 30;

    const float angle_step = M_PI * 2 / (float) steps;

    for(int i = 0; i < steps; i++) {
        float angle = angle_step * (float) i;
        Vec2f position = { sin(angle) * radius, cos(angle) * radius };
        Vec2f screen_position(position);
        screen_position += center;
        screen_position *= this->transform;
        vertex_buffer.push_back(fill_style->vertex(screen_position, position));
    }

    auto render_states = fill_style->get_render_states();
    if(render_states) target->get_target()->draw(&vertex_buffer[0], vertex_buffer.size(), sf::TriangleFan, *render_states);
    else target->get_target()->draw(&vertex_buffer[0], vertex_buffer.size(), sf::TriangleFan);
}

void DrawingContext::stroke_text(Vec2f position, Vec2f size, const char* text) const {
    position *= this->transform;

    sf::Text sfText;
    sfText.setCharacterSize(15);
    sfText.setString(text);
    sfText.setFillColor(text_color);
    sfText.setFont(*font);

    auto bounds = sfText.getLocalBounds();
    float height = bounds.height;
    float width = bounds.width;

    if(hAlignment == HTextAlignmentCenter) position.set_x(position[0] + size[0] / 2 - width / 2);
    else if(hAlignment == HTextAlignmentLeft) position.set_x(position[0]);
    else position.set_x(position[0] + size[0] - width);

    if(vAlignment == VTextAlignmentCenter) position.set_y(position[1] + size[1] / 2 - height / 2);
    else if(vAlignment == VTextAlignmentTop) position.set_y(position[1]);
    else position.set_y(position[1] + size[1] - height);

    sfText.setPosition({position[0], position[1]});

    target->get_target()->draw(sfText);
}

void DrawingContext::fill_rect(const Vec2f& position, const Vec2f& size) const {
    if(!fill_style) return;

    Vec2f tlVertex(position);
    Vec2f trVertex(position[0] + size[0], position[1]);
    Vec2f blVertex(position[0], position[1] + size[1]);
    Vec2f brVertex(position[0] + size[0], position[1] + size[1]);

    tlVertex *= transform;
    trVertex *= transform;
    blVertex *= transform;
    brVertex *= transform;

    sf::Vertex quad[] = {
        fill_style->vertex(blVertex, {0, 0}),
        fill_style->vertex(tlVertex, {0, size[1]}),
        fill_style->vertex(trVertex, {size[0], size[1]}),
        fill_style->vertex(brVertex, {size[0], 0})
    };

    auto render_states = fill_style->get_render_states();
    if(render_states) target->get_target()->draw(quad, 4, sf::Quads, *render_states);
    else target->get_target()->draw(quad, 4, sf::Quads);
}

void DrawingContext::fill_shape(const std::vector<Vertex> &shape, PrimitiveType primitive_type) {
    if(!fill_style) return;

    vertex_buffer.clear();

    for(int i = 0; i < shape.size(); i++) {
        Vec2f position = shape[i].position;
        position *= transform;
        vertex_buffer.push_back(fill_style->vertex(position, shape[i].shape_position));
    }

    auto type = (sf::PrimitiveType) primitive_type;

    auto render_states = fill_style->get_render_states();
    if(render_states) target->get_target()->draw(&vertex_buffer[0], shape.size(), type, *render_states);
    else target->get_target()->draw(&vertex_buffer[0], shape.size(), type);
}

DrawingTarget* DrawingContext::get_render_target() {
    return target;
}

void DrawingContext::pop_render_target() {
    target_stack.pop_back();
    if(!target_stack.empty()) {
        target = target_stack[target_stack.size() - 1];
    } else {
        target = nullptr;
    }
}

void DrawingContext::push_render_target(DrawingTarget* p_target) {
    target = p_target;
    target_stack.push_back(p_target);
}

void DrawingContext::clear(const Vec4f& color) {
    target->get_target()->clear(color.to_sf_color());
}
