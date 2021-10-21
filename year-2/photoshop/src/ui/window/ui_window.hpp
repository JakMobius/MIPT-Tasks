#pragma once

class UIWindow;

#include "../ui_view.hpp"
#include "ui_window_header.hpp"
#include "ui_window_container.hpp"
#include "../ui_stack.hpp"
#include "../styles/window_style.hpp"

class UIWindow : public UIStackView, public Styled<UIWindowStyle> {

    UIWindowContainer* container;
    UIWindowHeaderView* header_view;
    UIView* content_view = new UIView();
    float header_height = 40;

    void update_style();

public:
    explicit UIWindow(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}, const char* title = nullptr);

    void layout() override;

    void set_title(const char* string);
    void set_style(const UIWindowStyle* p_style) override;
    void set_active(bool p_is_active) override;

    UIView* get_content_view() { return content_view; }

    UIWindowContainer* get_container_view() const { return container; }
    void set_container_view(UIWindowContainer* p_container) { container = p_container; }

    void close();
};