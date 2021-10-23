//
// Created by Артем on 19.10.2021.
//

#include "ui_window_container.hpp"

void UIWindowContainer::remove_window(UIWindow* window) {
    if(window->get_container_view() != this) return;
    auto window_index = get_window_index(window);
    auto child_index = get_child_index(window);

    if(window_index == -1 || child_index == -1) return;

    if(window == active_window) activate_window(nullptr);

    window->set_container_view(nullptr);
    windows.erase(windows.begin() + window_index);
    remove_child(child_index);

    delete window;
}

void UIWindowContainer::add_window(UIWindow* window) {
    if(window->get_parent()) return;
    window->set_container_view(this);
    windows.push_back(window);
    append_child(window);
}

int UIWindowContainer::get_window_index(UIView* maybe_window) {
    for(int i = 0; i < windows.size(); i++) {
        if(windows[i] == maybe_window) return i;
    }
    return -1;
}

void UIWindowContainer::on_mouse_down(MouseDownEvent* event) {
    update_hovered_child(event->x, event->y);

    if(current_hovered_child != active_window) {
        if(current_hovered_child) {
            maybe_activate_child(current_hovered_child);
        } else activate_window(nullptr);
        set_needs_redraw();
        event->mark_handled();
    }

    UIView::on_mouse_down(event);
}

void UIWindowContainer::maybe_activate_child(UIView* view) {
    int index = get_window_index(view);
    if(index >= 0) activate_window(windows[index]);
    else activate_window(nullptr);
}

void UIWindowContainer::activate_window(UIWindow* window) {
    if(active_window) {
        active_window->set_active(false);
    }
    active_window = window;
    if(!window) return;

    window->set_active(true);
    int view_index = get_child_index(window);
    for(int i = view_index + 1; i < children.size(); i++) {
        children[i - 1] = children[i];
    }
    children[children.size() - 1] = active_window;
}
