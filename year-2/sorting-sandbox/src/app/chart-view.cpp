//
// Created by Артем on 02.02.2022.
//

#include "chart-view.hpp"
#include <sstream>

void ChartView::update_label_text(const Vec2f &position) {
    std::stringstream ss; ss << "X: " << position.x << ", Y: " << position.y;

    m_label_text = ss.str();
}

void ChartView::update_label() {
    Vec2f position = m_mouse_position;

    bool found_y = false;

    auto& chart = m_charts[m_active_chart_index];

    for(int i = 1; i < chart.points.size(); i++) {
        Vec2f point_a = from_graph_cs(chart.points[i - 1]);
        Vec2f point_b = from_graph_cs(chart.points[i]);

        Vec2f direction = point_b - point_a;
        float length = direction.len();
        direction /= length;

        float fraction = (position - point_a).dot(direction) / length;
        if(fraction < 0 || fraction > 1) continue;

        Vec2f projection = point_a + direction * fraction * length;

        float distance = (position - projection).len();
        if(distance > 30) break;
        found_y = true;

        position = projection;

        break;
    }

    if(found_y) {

        m_targeted_position = position;

        update_label_text(to_graph_cs(position));

        m_label->set_position(m_mouse_position + Vec2f(10, 10));
        m_label->get_text_drawer()->set_text(m_label_text.c_str());
        m_label->set_hidden(false);
    } else {
        m_targeted_position = {};
        m_label->set_hidden(true);
    }
}

void ChartView::draw(DrawingContext* ctx) {
    auto stroke_style = UIStrokeStyleColor({1, 1, 1, 1});
    ctx->set_stroke_style(&stroke_style);

    Vec2f center = from_graph_cs({0, 0});

    ctx->stroke_line({0, 0}, {0, m_size.y}, 2);
    ctx->stroke_line({0, 0}, {m_size.x, 0}, 2);
    ctx->stroke_line({m_size.x, m_size.y}, {0, m_size.y}, 2);
    ctx->stroke_line({m_size.x, m_size.y}, {m_size.x, 0}, 2);

    ctx->stroke_line({0, center.y}, {m_size.x, center.y}, 2);
    ctx->stroke_line({center.x, 0}, {center.x, m_size.y}, 2);

    for(int i = 0; i < m_charts.size(); i++) {
        auto& chart = m_charts[i];
        Vec4f color = chart.color;

        if(i != m_active_chart_index) color.w *= 0.5;

        stroke_style = UIStrokeStyleColor(color);
        ctx->set_stroke_style(&stroke_style);

        for(int i = 1; i < chart.points.size(); i++) {
            ctx->stroke_line(from_graph_cs(chart.points[i - 1]), from_graph_cs(chart.points[i]), 2);
        }
    }

    stroke_style = UIStrokeStyleColor({0.5, 0.5, 0.5, 1});
    ctx->set_stroke_style(&stroke_style);

    if(!m_targeted_position.is_zero()) {
        ctx->stroke_line({center.x, m_targeted_position.y}, m_targeted_position, 2);
        ctx->stroke_line({m_targeted_position.x, center.y}, m_targeted_position, 2);
    }
}

void ChartView::on_mouse_scroll(MouseScrollEvent* event) {
    UIView::on_mouse_scroll(event);

    if(event->m_modifiers.control) {
        Vec2f mouse_position = {event->x, event->y};

        float coefficient = 1.0f + (float) event->dy * 0.1;

        if(coefficient < 0.9f) coefficient = 0.9f;
        if(coefficient > 1.1f) coefficient = 1.1f;

        m_camera = (mouse_position + m_camera) * Vec2f { coefficient, coefficient } - mouse_position;
        m_scale *= Vec2f { coefficient, coefficient };

    } else {
        m_camera -= Vec2f {(float) event->dx, (float) event->dy} * 5;
    }

    hide_overlay();
}

void ChartView::on_mouse_out(MouseOutEvent* event) {
    UIView::on_mouse_out(event);

    hide_overlay();
}

void ChartView::on_mouse_move(MouseMoveEvent* event) {
    UIView::on_mouse_move(event);

    m_mouse_position = {event->x, event->y };
    update_label();
    set_needs_redraw();
}

void ChartView::hide_overlay() {
    m_targeted_position = {};
    m_label->set_hidden(true);
    set_needs_redraw();
}
