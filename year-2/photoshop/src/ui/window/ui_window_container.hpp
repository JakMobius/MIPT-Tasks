#pragma once

class UIWindowContainer;

#import "../ui_view.hpp"
#include "ui_window.hpp"

class UIWindowContainer : public UIView {
private:
    UIFillStyleColor background_style {{ 0.2, 0.2, 0.2, 1 }};
    UIWindow* active_window = nullptr;
    std::vector<UIWindow*> windows {};

    void maybe_activate_child(UIView* view);

protected:
    void activate_window(UIWindow* window);

public:
    explicit UIWindowContainer(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}): UIView(position, size) {}
    void add_window(UIWindow* window);
    void remove_window(UIWindow* window);
    int get_window_index(UIView* maybe_window);
    void on_mouse_down(MouseDownEvent *event) override;
    void layout() override;
};