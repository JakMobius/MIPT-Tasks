#pragma once

#include "../ui/window/ui_window.hpp"
#include "app-view.hpp"
#include "smart-int.hpp"

class ControlsWindow : public UIWindow {

    AppView* m_app_view;

public:
    explicit ControlsWindow(AppView* app_view);

    void open_animation(int sort_index);
    void open_chart(int sort_index);

    static void sort_indexed(std::vector<SmartInt> &vector, int index);

    void create_charts();

    Vec4f ith_color(int i, bool light);
};