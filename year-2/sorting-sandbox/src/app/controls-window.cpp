//
// Created by Артем on 03.02.2022.
//


#include <random>
#include "controls-window.hpp"
#include "smart-int.hpp"
#include "bubble-sort.hpp"
#include "selection-sort.hpp"
#include "../utils/hsv.hpp"

static const char* CHART_NAMES[] = {
    "std::sort",
    "std::stable_sort",
    "bubble sort",
    "selection sort"
};

ControlsWindow::ControlsWindow(AppView* app_view) : UIWindow({200, 200}, {600, 120}, "Controls"), m_app_view(app_view) {
    auto stack_view = new UIStackView(UIAxis::x);
    stack_view->set_item_spacing(10);
    stack_view->set_insets({ 10 });

    for(int i = 0; i < sizeof(CHART_NAMES) / sizeof(char*); i++) {
        auto button = new UIButton({}, {130, 100});
        button->set_title(CHART_NAMES[i]);
        button->set_callback([this, i]() {
            open_chart(i);
        });
        stack_view->append_child(button);

        button->set_text_color(ith_color(i, false));
    }

    set_can_be_closed(false);

    get_content_view()->append_child(stack_view);
}

void ControlsWindow::open_chart(int sort_index) {

    std::random_device dev {};
    std::mt19937 random { dev() };

    auto* chart_window = m_app_view->get_chart_window();
    if(!chart_window) {
        create_charts();

        chart_window = m_app_view->get_chart_window();
    }

    auto& chart_a = chart_window->get_chart_view_a()->get_chart(sort_index).points;
    auto& chart_b = chart_window->get_chart_view_b()->get_chart(sort_index).points;

    chart_a.clear();
    chart_b.clear();

    for(int i = 0; i < 2048; i += 128) {

        StatisticsCollector statistics {};
        std::vector<SmartInt> smart_ints_array { (size_t) i, &statistics };

        for(int j = 0; j < i; j++) {
            smart_ints_array[j].set_value(random());
        }

        sort_indexed(smart_ints_array, sort_index);

        chart_a.emplace_back((float) i, (float)statistics.m_comparisons);
        chart_b.emplace_back((float) i, (float)statistics.m_assignments);
    }

    chart_window->get_chart_view_a()->set_active_chart(sort_index);
    chart_window->get_chart_view_b()->set_active_chart(sort_index);
}

void ControlsWindow::sort_indexed(std::vector<SmartInt> &vector, int index) {
    switch(index) {
        case 0:
            std::sort(vector.begin(), vector.end());
            break;
        case 1:
            std::stable_sort(vector.begin(), vector.end());
            break;
        case 2:
            bubble_sort(vector.begin(), vector.end());
            break;
        case 3:
            selection_sort(vector.begin(), vector.end());
        default:
            break;
    }
}

Vec4f ControlsWindow::ith_color(int i, bool light) {
    int charts = sizeof(CHART_NAMES) / sizeof(char*);

    float hue = 360.f / (float) charts * (float) i;

    Vec4f color;

    if(light) color = Vec4f { rgb_from_hsv({hue, 0.5f, 1.0f}).m_content };
    else      color = Vec4f { rgb_from_hsv({hue, 1.0f, 0.5f}).m_content };

    color.w = 1.0;

    return color;
}

void ControlsWindow::create_charts() {
    m_app_view->create_chart_window();

    int charts = sizeof(CHART_NAMES) / sizeof(char*);
    for(int i = 0; i < charts; i++) {
        auto color = ith_color(i, true);
        m_app_view->get_chart_window()->get_chart_view_a()->add_chart({{}, color});
        m_app_view->get_chart_window()->get_chart_view_b()->add_chart({{}, color});
    }
}
