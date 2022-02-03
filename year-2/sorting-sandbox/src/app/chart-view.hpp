#pragma once

#include "../ui/ui_view.hpp"
#include "../ui/ui_text.hpp"

struct Chart {
    std::vector<Vec2f> points;
    Vec4f color;
};

class ChartView : public UIView {

    std::vector<Chart> m_charts {};

    Vec2f m_camera {};
    Vec2f m_scale {};
    Vec2f m_mouse_position {};
    Vec2f m_targeted_position {};

    int m_active_chart_index = 0;

    UIText* m_label = nullptr;
    std::string m_label_text {};

public:
    explicit ChartView(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}): UIView(position, size) {
        m_scale = { 1, -1.0f / 30 };
        m_label = new UIText();
        m_label->set_hidden(true);
        m_label->set_interactions_enabled(false);
        set_masks_to_bounds(true);

        m_camera = { 0, -size.y };

        append_child(m_label);
    }

    void hide_overlay();

    void set_active_chart(int index) { m_active_chart_index = index; set_needs_redraw(); }

    void add_chart(const Chart& chart) { m_charts.push_back(chart); }
    void add_chart(Chart&& chart) { m_charts.push_back(chart); }
    Chart& get_chart(int i) { return m_charts[i]; }

    void on_mouse_move(MouseMoveEvent* event) override;

    void on_mouse_out(MouseOutEvent *event) override;

    void update_label();

    void on_mouse_scroll(MouseScrollEvent *event) override;

    Vec2f to_graph_cs(const Vec2f& position) {
        return (position + m_camera) /= m_scale;
    }

    Vec2f from_graph_cs(const Vec2f& position) {
        return (position * m_scale) -= m_camera;
    }

    void update_label_text(const Vec2f &position);

    void draw(DrawingContext *ctx) override;
};