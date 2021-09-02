//
// Created by Артем on 01.09.2021.
//

#include "graph.hpp"

#include <utility>

Graph::Graph(float (*func)(float x), Vec2 position, Vec2 size): graph_position(std::move(position)),
                                                                graph_size(std::move(size)),
                                                                vectors_to_draw() {
    vectors_to_draw.construct();
    f = func;
    set_viewport({10, 10});
}

Graph::~Graph() {
    vectors_to_draw.destruct();
}

Graph::Graph(Graph &&other) noexcept: graph_position(std::move(other.graph_position)),
                                      graph_size(std::move(other.graph_size)),
                                      f(other.f),
                                      vectors_to_draw() {
    vectors_to_draw.construct(&other.vectors_to_draw);
}

Graph &Graph::operator=(Graph &&other) noexcept {
    if(this == &other) return *this;
    graph_position = other.graph_position;
    graph_size = other.graph_size;

    return *this;
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

    ctx->color = {0, 0, 0, 255};

    float arrow_size = 10;

    if(center_position.y > 0 && center_position.y < graph_size.y) {
        ctx->draw_line({0, center_position.y}, {graph_size.x, center_position.y});
        ctx->draw_line({graph_size.x, center_position.y}, {graph_size.x - arrow_size, center_position.y + arrow_size});
        ctx->draw_line({graph_size.x, center_position.y}, {graph_size.x - arrow_size, center_position.y - arrow_size});
    }

    if(center_position.x > 0 && center_position.x < graph_size.x) {
        ctx->draw_line({center_position.x, 0}, {center_position.x, graph_size.y});
        ctx->draw_line({center_position.x, 0}, {center_position.x - arrow_size, arrow_size});
        ctx->draw_line({center_position.x, 0}, {center_position.x + arrow_size, arrow_size});
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

    float leftPosition = Vec2(leftPoint).transform(inverse_coordinate_space).x;
    float rightPosition = Vec2(rightPoint).transform(inverse_coordinate_space).x;
    float topPosition = Vec2(topPoint).transform(inverse_coordinate_space).y;
    float bottomPosition = Vec2(bottomPoint).transform(inverse_coordinate_space).y;

    leftPoint  .add({ text_offset,  text_offset });
    rightPoint .add({ -text_offset, text_offset });
    topPoint   .add({ text_offset,  text_offset });
    bottomPoint.add({ text_offset, -text_offset });

    ctx->color = {255, 0, 0, 255};
    ctx->vAlignment = VTextAlignment::bottom;
    ctx->hAlignment = HTextAlignment::left;
    draw_text(ctx, rightPosition, rightPoint);
    ctx->hAlignment = HTextAlignment::right;
    draw_text(ctx, leftPosition, leftPoint);
    draw_text(ctx, topPosition, topPoint);
    ctx->vAlignment = VTextAlignment::top;
    draw_text(ctx, bottomPosition, bottomPoint);
}

void Graph::draw_background(DrawingContext* ctx) const {
    ctx->color = {255, 255, 255, 255};
    ctx->draw_rect({0, 0}, graph_size);
}

void Graph::draw(DrawingContext* ctx) const {
    Matrix3 saved = ctx->transform;

    transform_matrix(ctx);
    draw_background(ctx);
    draw_zeroes(ctx);
    draw_labels(ctx);
    draw_function(ctx);

    ctx->color = {0, 0, 255, 255};

    for(int i = 0; i < vectors_to_draw.size; i++) {
        draw_vector(ctx, vectors_to_draw.get(i));
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

static bool clampValue(float* value, float min, float max) {
    if(*value < min) *value = min;
    else if(*value > max) *value = max;
    else return false;
    return true;
}

void Graph::draw_function(DrawingContext* ctx) const {
    Matrix3 saved = ctx->transform;

    ctx->transform = coordinate_space.multiplied(ctx->transform);

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

    int steps = 200;
    float step = (toX - fromX) / (float)steps;
    float x = fromX;

    Vec2 oldPosition = Vec2(x, f(x));
    bool old_position_invalid = clampValue(&oldPosition.y, fromY, toY);

    while(true) {

        x += step;
        Vec2 newPosition = Vec2(x, f(x));
        bool position_invalid = clampValue(&newPosition.y, fromY, toY);

        if(!position_invalid || !old_position_invalid) {
            ctx->draw_line(oldPosition, newPosition);

            if(x > toX) {
                x = toX;
                oldPosition = newPosition;
                old_position_invalid = position_invalid;

                newPosition = Vec2(x, f(x));
                position_invalid = clampValue(&newPosition.y, fromY, toY);

                if(!position_invalid || !old_position_invalid) {
                    ctx->draw_line(oldPosition, newPosition);
                }
                break;
            }
        } else if(x > toX) break;

        oldPosition = newPosition;
        old_position_invalid = position_invalid;
    }

    ctx->transform = saved;

}

void Graph::transform_matrix(DrawingContext* ctx) const {
    ctx->transform.translate(-graph_size.x / 2, -graph_size.y / 2);
    ctx->transform.rotate(graph_angle);
    ctx->transform.translate(graph_position.x + graph_size.x / 2, graph_position.y + graph_size.y / 2);
}

void Graph::draw_vector(DrawingContext* ctx, const VisualVector &vector) const {
    const float arrow_size = 10;

    Vec2 from(vector.base);
    Vec2 to(vector.projections);

    from.transform(coordinate_space);
    to.transform(coordinate_space, 0);

    Vec2 projectionsTransformed(to);

    to.add(from);

    ctx->draw_line(from, to);

    if(vector.style == VisualVectorStyle::arrowed) {
        Vec2 arrow_offset = projectionsTransformed;
        float arrow_offset_length = arrow_offset.length();

        arrow_offset.multiply(-arrow_size / arrow_offset_length);

        Vec2 arrow_point = to;
        arrow_point.add(arrow_offset);

        Vec2 arrow_offset_orthogonal(-arrow_offset.y, arrow_offset.x);

        ctx->draw_line(to, arrow_point.adding(arrow_offset_orthogonal));
        ctx->draw_line(to, arrow_point.subtracting(arrow_offset_orthogonal));

    } else if(vector.style == VisualVectorStyle::circled) {
        ctx->draw_circle(to, 10);
    }
}

VisualVector::VisualVector(const Vec2& base, const Vec2 &projections, VisualVectorStyle style): base(base), projections(projections), style(style) {}
