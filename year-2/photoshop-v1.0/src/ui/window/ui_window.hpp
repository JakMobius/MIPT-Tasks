#pragma once

class UIWindow;

#include "../ui_view.hpp"
#include "ui_window_header.hpp"
#include "ui_window_container.hpp"
#include "../ui_stack.hpp"
#include "../styles/window_style.hpp"
#include "../../events/event_emitter.hpp"
#include "../../graphics/shapes/rounded_rect_shape.hpp"

struct WindowCloseEvent {
    UIWindow* window;
};

class UIWindow : public UIView, public Styled<UIWindowStyle> {

    UIWindowContainer* container = nullptr;
    UIWindowHeaderView* header_view = nullptr;
    UIView* inner_view = new UIStackView(UIAxis::y);
    UIView* content_view = new UIView();
    RoundedRectShape* inner_shape = nullptr;

    EventEmitter<WindowCloseEvent> close_event_emitter {};

    float header_height = 42;

    void update_style();

public:
    explicit UIWindow(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}, const char* title = nullptr);
    ~UIWindow();
    void layout() override;

    void set_title(const char* string);
    void set_style(const UIWindowStyle* p_style) override;

    void set_size(const Vec2f &new_size) override;

    UIView* get_content_view() { return content_view; }

    UIWindowContainer* get_container_view() const { return container; }
    void set_container_view(UIWindowContainer* p_container) { container = p_container; }

    EventEmitter<WindowCloseEvent>* get_close_event_emitter() { return &close_event_emitter; }

    void on_mouse_down(MouseDownEvent *event)   override;
    void on_mouse_up(MouseUpEvent *event)       override;
    void on_mouse_move(MouseMoveEvent *event)   override;

    void handle_child_blur() override { current_focused_child = nullptr; }
    void focus() override;
    void blur() override;

    void close();
};