#pragma once

class AppView;

#include "../utils/vec4.hpp"
#include "../ui/ui_view.hpp"
#include "../ui/window/ui_window.hpp"
#include "../ui/window/ui_window_container.hpp"
#include "chart-window.hpp"
#include "statistics-collector.hpp"
#include <functional>

class ColorPickerWindow;
class App;

class AppView : public UIView {

    static UIFillStyleColor m_app_background;

    UIWindowContainer* m_window_container = new UIWindowContainer();
    ChartWindow* m_chart_window = nullptr;
    App* m_app = nullptr;

public:
    explicit AppView(App* app, const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0});

    void create_chart_window();
    ChartWindow* get_chart_window();
    void layout() override;

    void present_window_modally(UIWindow* window);
    void present_window(UIWindow* window);
    void focus_window(UIWindow* window);

    UIWindowContainer* get_window_container() { return m_window_container; }

};