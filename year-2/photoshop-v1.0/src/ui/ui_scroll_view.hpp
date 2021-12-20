#pragma once

#include "../utils/vec2.hpp"
#include "ui_view.hpp"

class UIScrollView : public UIView {
    UIView* content_view;
    Vec2f content_size {};
    Vec2f scroll_position {};

    void update_content_size();

public:
    UIScrollView(const Vec2f& position, const Vec2f& size);

    void scroll_to(const Vec2f& position);

    void on_mouse_scroll(MouseScrollEvent *event) override;

    void layout() override;

    UIView* get_content_view() { return content_view; }
};