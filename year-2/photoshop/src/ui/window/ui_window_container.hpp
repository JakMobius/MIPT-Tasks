#pragma once

#import "../ui_view.hpp"
#include "ui_window.hpp"

class UIWindowContainer : public UIView {
private:

    UIWindow* active_window = nullptr;
    std::vector<UIWindow*> windows {};

    void activate_window(UIWindow* window) {
        active_window = window;
        if(!window) return;

        window->set_is_active(true);
        int view_index = get_child_index(window);
        for(int i = children.size() - 1; i > view_index; i--) {
            children[i - 1] = children[i];
        }
        children[children.size() - 1] = active_window;
    }

    void maybe_activate_child(UIView* view) {
        int index = get_window_index(view);
        if(index >= 0) activate_window(windows[index]);
        else activate_window(nullptr);
    }

public:
    explicit UIWindowContainer(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}): UIView(position, size) {}

    void add_window(UIWindow* window) {
        windows.push_back(window);
        append_child(window);
    }

    int get_window_index(UIView* maybe_window) {
        for(int i = 0; i < windows.size(); i++) {
            if(windows[i] == maybe_window) return i;
        }
        return -1;
    }

    void on_mouse_down(MouseDownEvent *event) override {
        update_hovered_child(event->x, event->y);

        if(current_hovered_child != active_window) {
            if(active_window) {
                active_window->set_is_active(false);
            }
            if(current_hovered_child) {
                maybe_activate_child(current_hovered_child);
            }
            set_needs_redraw();
            event->mark_handled();
        }

        UIView::on_mouse_down(event);
    }
};
