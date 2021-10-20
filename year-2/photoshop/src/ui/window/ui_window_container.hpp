#pragma once

class UIWindowContainer;

#import "../ui_view.hpp"
#include "ui_window.hpp"

class UIWindowContainer : public UIView {
private:

    UIWindow* active_window = nullptr;
    std::vector<UIWindow*> windows {};

    void activate_window(UIWindow* window);

    void maybe_activate_child(UIView* view);

public:
    explicit UIWindowContainer(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}): UIView(position, size) {}
    void add_window(UIWindow* window);
    void remove_window(UIWindow* window);
    int get_window_index(UIView* maybe_window);
    void on_mouse_down(MouseDownEvent *event) override;
};
