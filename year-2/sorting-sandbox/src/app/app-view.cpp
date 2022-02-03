//
// Created by Артем on 18.10.2021.
//

#include <random>
#include "app-view.hpp"
#include "../ui/ui_modal_view.hpp"
#include "../ui/ui_screen.hpp"
#include "chart-view.hpp"
#include "chart-window.hpp"
#include "smart-int.hpp"
#include "controls-window.hpp"

UIFillStyleColor AppView::m_app_background {{0.2, 0.2, 0.2, 1 }};

AppView::AppView(App* app, const Vec2f &position, const Vec2f &size) : UIView(position, size), m_app(app) {

    auto* window = new ControlsWindow(this);
    present_window(window);

    append_child(m_window_container);
}

void AppView::layout() {
    m_window_container->set_size(m_size - Vec2f {0, 25});
    if(!m_fill_style) set_fill_style(&m_app_background);
    UIView::layout();
}

void AppView::present_window_modally(UIWindow* window) {
    auto screen = get_screen();
    auto nested_window_container = new UIWindowContainer();
    auto overlay = new UIModalViewOverlay();

    overlay->set_stretched_child(nested_window_container);

    nested_window_container->add_window(window);

    window->layout();
    window->set_position((screen->get_size() * 0.5) -= (window->get_size() * 0.5));

    auto* window_destroy_event = new EventHandler<WindowCloseEvent>;
    auto* overlay_destroy_handler = new EventHandler<ViewDestroyEvent>;

    *window_destroy_event = [window_destroy_event, overlay, overlay_destroy_handler](WindowCloseEvent*){
        overlay->get_destroy_event_emitter()->remove_listener(overlay_destroy_handler);
        overlay->get_container()->close_modal(overlay);
        delete window_destroy_event;
    };

    *overlay_destroy_handler = [window, window_destroy_event](ViewDestroyEvent*) {
        window->get_close_event_emitter()->remove_listener(window_destroy_event);
    };

    overlay->get_destroy_event_emitter()->add_listener(overlay_destroy_handler);
    window->get_close_event_emitter()->add_listener(window_destroy_event);

    screen->present_modal(overlay);

    nested_window_container->focus_window(window);
}

void AppView::present_window(UIWindow* window) {
    m_window_container->add_window(window);
}

void AppView::focus_window(UIWindow* window) {
    m_window_container->focus_window(window);
}

ChartWindow* AppView::get_chart_window() {
    return m_chart_window;
}

void AppView::create_chart_window() {
    m_chart_window = new ChartWindow({100, 100}, "Chart");

    present_window(m_chart_window);
}
