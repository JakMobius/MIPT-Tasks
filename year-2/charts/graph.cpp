//
// Created by Артем on 01.09.2021.
//

#include "graph.hpp"

#include <utility>

Graph::Graph(float (*func)(float x), const Vec2& position, const Vec2& size): graph_position(position),
                                                                              graph_size(size),
                                                                              vectors_to_draw(),
                                                                              f(func),
                                                                              steps(200) {
    set_viewport(Vec2(10, 10));
}

Graph::~Graph() {

}

void Graph::draw_text(DrawingContext* ctx, float value, const Vec2 &text_position) const {
    const int buffer_size = 16;
    char buffer[buffer_size];
    snprintf(buffer, buffer_size - 1, "%.1f", value);
    ctx->draw_text(text_position, buffer);
}

void Graph::draw_zeroes(DrawingContext* ctx) const {
    Vec2 center_position(0, 0);
    center_position.transform(coordinate_space);

    ctx->color = sf::Color(0, 0, 0);

    float arrow_size = 10;

    if(center_position.y > 0 && center_position.y < graph_size.y) {
        ctx->draw_line(Vec2(0,            center_position.y), Vec2(graph_size.x,              center_position.y));
        ctx->draw_line(Vec2(graph_size.x, center_position.y), Vec2(graph_size.x - arrow_size, center_position.y + arrow_size));
        ctx->draw_line(Vec2(graph_size.x, center_position.y), Vec2(graph_size.x - arrow_size, center_position.y - arrow_size));
    }

    if(center_position.x > 0 && center_position.x < graph_size.x) {
        ctx->draw_line(Vec2(center_position.x, 0), Vec2(center_position.x,              graph_size.y));
        ctx->draw_line(Vec2(center_position.x, 0), Vec2(center_position.x - arrow_size, arrow_size));
        ctx->draw_line(Vec2(center_position.x, 0), Vec2(center_position.x + arrow_size, arrow_size));
    }
}

void Graph::draw_labels(DrawingContext* ctx) const {
    float text_offset = 10;

    Vec2 center_position(0, 0);
    center_position.transform(coordinate_space);

    Vec2 leftPoint(0, center_position.y);
    Vec2 rightPoint(graph_size.x, center_position.y);
    Vec2 topPoint(center_position.x, 0);
    Vec2 bottomPoint(center_position.x, graph_size.y);

    float leftPosition   = inverse_coordinate_space.transform_x(leftPoint.x,   leftPoint.y);
    float rightPosition  = inverse_coordinate_space.transform_x(rightPoint.x,  rightPoint.y);
    float topPosition    = inverse_coordinate_space.transform_y(topPoint.x,    topPoint.y);
    float bottomPosition = inverse_coordinate_space.transform_y(bottomPoint.x, bottomPoint.y);

    leftPoint  .add(Vec2( text_offset,  text_offset ));
    rightPoint .add(Vec2( -text_offset, text_offset ));
    topPoint   .add(Vec2( text_offset,  text_offset ));
    bottomPoint.add(Vec2( text_offset, -text_offset ));

    ctx->color = sf::Color(255, 0, 0);
    ctx->vAlignment = VTextAlignmentBottom;
    ctx->hAlignment = HTextAlignmentLeft;
    draw_text(ctx, rightPosition, rightPoint);
    ctx->hAlignment = HTextAlignmentRight;
    draw_text(ctx, leftPosition, leftPoint);
    draw_text(ctx, topPosition, topPoint);
    ctx->vAlignment = VTextAlignmentTop;
    draw_text(ctx, bottomPosition, bottomPoint);
}

void Graph::draw_background(DrawingContext* ctx) const {
    ctx->color = sf::Color(255, 255, 255, 255);
    ctx->draw_rect(Vec2(0, 0), graph_size);
}

void Graph::draw(DrawingContext* ctx) const {
    Matrix3 saved = ctx->transform;

    interface_coordinate_system(ctx);
    draw_background(ctx);
    draw_zeroes(ctx);
    draw_labels(ctx);
    algebraic_coordinate_system(ctx);
    draw_function(ctx);

    ctx->color = sf::Color(0, 0, 255);

    for(int i = 0; i < vectors_to_draw.size; i++) {
        vectors_to_draw.get(i)->draw(ctx);
    }

    ctx->transform = saved;
}

void Graph::set_viewport(const Vec2 &scale, const Vec2 &center, float angle) {
    viewport_center = center;
    viewport_scale = scale;
    graph_angle = angle;

    coordinate_space.reset();
    coordinate_space.translate(-viewport_center.x, -viewport_center.y);
    coordinate_space.scale(viewport_scale.x, -viewport_scale.y);
    coordinate_space.translate( graph_size.x / 2,  graph_size.y / 2);
    inverse_coordinate_space = coordinate_space.inverse();
}

static bool clamp_value(float* value, float min, float max) {
    if(*value < min) *value = min;
    else if(*value > max) *value = max;
    else return false;
    return true;
}

void Graph::draw_function(DrawingContext* ctx) const {

    if(f == nullptr) return;

    float fromX = inverse_coordinate_space.transform_x(0, 0);
    float toX = inverse_coordinate_space.transform_x(graph_size.x, graph_size.y);
    float fromY = inverse_coordinate_space.transform_y(0, 0);
    float toY = inverse_coordinate_space.transform_y(graph_size.x, graph_size.y);

    if(fromY > toY) {
        float temp = toY;
        toY = fromY;
        fromY = temp;
    }

    float step = (toX - fromX) / (float)steps;
    float x = fromX;

    Vec2 oldPosition = Vec2(x, f(x));
    bool old_position_invalid = clamp_value(&oldPosition.y, fromY, toY);

    while(true) {

        x += step;
        Vec2 newPosition = Vec2(x, f(x));
        bool position_invalid = clamp_value(&newPosition.y, fromY, toY);

        if(!position_invalid || !old_position_invalid) {
            ctx->draw_line(oldPosition, newPosition);

            if(x > toX) {
                x = toX;
                oldPosition = newPosition;
                old_position_invalid = position_invalid;

                newPosition = Vec2(x, f(x));
                position_invalid = clamp_value(&newPosition.y, fromY, toY);

                if(!position_invalid || !old_position_invalid) {
                    ctx->draw_line(oldPosition, newPosition);
                }
                break;
            }
        } else if(x > toX) break;

        oldPosition = newPosition;
        old_position_invalid = position_invalid;
    }
}

void Graph::interface_coordinate_system(DrawingContext* ctx) const {
    ctx->transform.translate(-graph_size.x / 2, -graph_size.y / 2);
    ctx->transform.rotate(graph_angle);
    ctx->transform.translate(graph_position.x + graph_size.x / 2, graph_position.y + graph_size.y / 2);
}

void Graph::algebraic_coordinate_system(DrawingContext* ctx) const {
    ctx->transform = coordinate_space.multiplied(ctx->transform);
}

void draw_vector_c_style(Graph* thou, DrawingContext* ctx, const VisualVector* vector) {
    const float arrow_size = 10;

    Vec2 from(vector->base.x, vector->base.y);
    Vec2 to(vector->projections.x, vector->projections.y);

    const Matrix3& coordinate_space = thou->get_coordinate_space();

    from.x = coordinate_space.transform_x(from.x, from.y);
    from.y = coordinate_space.transform_y(from.x, from.y);
    to.x = coordinate_space.transform_x(to.x, to.y, 0);
    to.x = coordinate_space.transform_x(to.x, to.y, 0);

    Vec2 projectionsTransformed(to.x, to.y);

    to.x += from.x;
    to.y += from.y;

    ctx->draw_line(from, to);

    if(vector->style == VisualVectorStyleArrowed) {
        Vec2 arrow_offset(projectionsTransformed.x, projectionsTransformed.y);

        float arrow_offset_length = sqrt(arrow_offset.x * arrow_offset.x + arrow_offset.y * arrow_offset.y);
        float length_coefficient = -arrow_size / arrow_offset_length;

        arrow_offset.x *= length_coefficient;
        arrow_offset.y *= length_coefficient;

        Vec2 arrow_offset_orthogonal(-arrow_offset.y, arrow_offset.x);
        Vec2 arrow_point(to.x + arrow_offset.x + arrow_offset_orthogonal.x, to.y + arrow_offset.y + arrow_offset_orthogonal.y);

        ctx->draw_line(to, arrow_point);

        arrow_point.x -= arrow_offset_orthogonal.x * 2;
        arrow_point.y -= arrow_offset_orthogonal.x * 2;

        ctx->draw_line(to, arrow_point);

    } else if(vector->style == VisualVectorStyleCircled) {
        ctx->draw_circle(to, 10);
    }
}

VisualVector::VisualVector(const Vec2& base, const Vec2 &projections, VisualVectorStyle style): base(base), projections(projections), style(style) {}

void VisualVector::draw(DrawingContext* ctx) const {
    Vec2 from(base);
    Vec2 to(projections);

    to += from;

    ctx->draw_line(from, to);

    switch(style) {
        case VisualVectorStyleArrowed: {
            Vec2 arrow_offset = -projections * 0.2;
            Vec2 arrow_point = to + arrow_offset;

            ctx->draw_line(to, arrow_point + Vec2( -arrow_offset.y, arrow_offset.x ) * 0.2);
            ctx->draw_line(to, arrow_point + Vec2( arrow_offset.y, -arrow_offset.x ) * 0.2);
            break;
        }
        case VisualVectorStyleCircled: {
            ctx->draw_circle(to, projections.length() / 5);
            break;
        }
    }
}
