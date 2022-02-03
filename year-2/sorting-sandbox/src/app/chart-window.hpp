#pragma once

#include "../ui/window/ui_window.hpp"
#include "chart-view.hpp"

class ChartWindow : public UIWindow {

    ChartView* m_chart_a = nullptr;
    ChartView* m_chart_b = nullptr;

public:
    explicit ChartWindow(const Vec2f& position = {0, 0}, const char* title = nullptr):
        UIWindow(position, {830, 470}, title) {

        auto text_a = new UIText({15, 15}, {400, 30}, "Comparisons chart");
        auto text_b = new UIText({425, 15}, {400, 30}, "Assignments chart");

        text_a->get_text_drawer()->set_font_size(23);
        text_b->get_text_drawer()->set_font_size(23);

        m_chart_a = new ChartView({10, 60}, {400, 400});
        m_chart_b = new ChartView({420, 60}, {400, 400});

        get_content_view()->append_child(m_chart_a);
        get_content_view()->append_child(m_chart_b);
        get_content_view()->append_child(text_a);
        get_content_view()->append_child(text_b);

        m_chart_a->set_needs_redraw();
        m_chart_b->set_needs_redraw();

    }

    ChartView* get_chart_view_a() { return m_chart_a; }
    ChartView* get_chart_view_b() { return m_chart_b; }

};