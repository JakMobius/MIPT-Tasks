#pragma once

class UIModalContainerView;

#include "ui_view.hpp"
#include "ui_modal_view.hpp"

class UIModalContainerView : public UIView {
    std::vector<UIModalView*> modal_views_stack {};

    void close_single_modal(int index);

public:
    explicit UIModalContainerView(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}): UIView(position, size) {};

    void on_mouse_click(MouseClickEvent *event) override;
    void close_modal(UIModalView* view);
    void present() { set_hidden(false); }
    void hide();

    void present_modal(UIModalView* modal_view);
};