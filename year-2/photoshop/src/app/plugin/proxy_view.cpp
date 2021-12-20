//
// Created by Артем on 15.12.2021.
//

#include "proxy_view.hpp"
#include "host_render_target.hpp"

void ProxyView::on_mouse_down(MouseDownEvent* event) {
    UIView::on_mouse_down(event);
    if(!widget) return;
    PUPPY::Event::MousePress widget_event { {event->x, event->y}, PUPPY::MouseButton::left };
    widget->on_mouse_press(widget_event);
    old_position = {event->x, event->y};
    position_valid = true;
}

void ProxyView::on_mouse_up(MouseUpEvent* event) {
    UIView::on_mouse_up(event);
    if(!widget) return;
    PUPPY::Event::MouseRelease widget_event { {event->x, event->y}, PUPPY::MouseButton::left };
    widget->on_mouse_release(widget_event);
}

void ProxyView::on_mouse_out(MouseOutEvent* event) {
    UIView::on_mouse_out(event);
    if(!widget) return;
    position_valid = false;
}

void ProxyView::on_mouse_move(MouseMoveEvent* event) {
    UIView::on_mouse_move(event);
    if(!widget) return;
    if(!position_valid) {
        old_position = {event->x, event->y};
    }

    PUPPY::Event::MouseMove widget_event { {old_position[0], old_position[1]}, {event->x, event->y} };
    widget->on_mouse_move(widget_event);

    if(position_valid) {
        old_position = {event->x, event->y};
    }
}

void ProxyView::on_key_down(KeyDownEvent* event) {
    UIView::on_key_down(event);
    if(!widget) return;
    PUPPY::Event::KeyDown widget_event { (PUPPY::Keyboard::Key) (sf::Keyboard::Key) event->code };
    widget->on_key_down(widget_event);
}

void ProxyView::on_key_up(KeyUpEvent* event) {
    UIView::on_key_up(event);
    if(!widget) return;
    PUPPY::Event::KeyUp widget_event { (PUPPY::Keyboard::Key) (sf::Keyboard::Key) event->code };
    widget->on_key_up(widget_event);
}

void ProxyView::draw(DrawingContext* ctx) {
    UIView::draw(ctx);
    if(!widget) return;

    RawHostWidget::draw_internal(ctx, widget, false);

    if(widget->get_owner() && widget->get_owner()->widgets_should_tick) {
        set_needs_redraw();
    }
}

void ProxyView::on_mouse_scroll(MouseScrollEvent* event) {
    UIView::on_mouse_scroll(event);

    PUPPY::Event::Scroll widget_event {{event->dx, event->dy}, {event->x, event->y}};
    widget->on_scroll(widget_event);
}

void ProxyView::on_text_enter(TextEnterEvent* event) {
    UIView::on_text_enter(event);

    PUPPY::Event::TextEnter widget_event { event->get_unicode() };
    widget->on_text_enter(widget_event);
}
