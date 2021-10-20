#pragma once

class UIWindow;

#include "../ui_view.hpp"
#include "ui_window_header.hpp"
#include "../ui_drawing_context.hpp"
#include "ui_window_container.hpp"
#include "../ui_stack.hpp"
#include "../styles/window_style.hpp"

class UIWindow : public UIStackView {

    UIWindowContainer* container;
    UIWindowHeaderView* header_view;
    UIView* content_view = new UIView();
    const UIWindowStyle* style = nullptr;
    float header_height = 40;
    bool is_active = false;

public:
    explicit UIWindow(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}, const char* title = nullptr);

    void layout() override;

    void set_title(const char* string);
    void set_style(const UIWindowStyle* p_style);
    const UIWindowStyle* get_style() { return style; }

    void prepare_to_draw(UIDrawingContext* ctx) override {
        bool old_is_active = ctx->get_context_active();
        ctx->set_context_active(is_active);
        UIView::prepare_to_draw(ctx);
        ctx->set_context_active(old_is_active);
    }

    bool get_is_active() const { return is_active; };
    void set_is_active(bool p_is_active) { is_active = p_is_active; set_style(style); }

    UIView* get_content_view() { return content_view; }

    UIWindowContainer* get_container_view() const { return container; }
    void set_container_view(UIWindowContainer* p_container) { container = p_container; }

    void close();
};