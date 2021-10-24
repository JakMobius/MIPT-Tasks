//
// Created by Артем on 18.10.2021.
//

#include "ui_window.hpp"
#include "../../utils/dispatch_queue.hpp"
#include "../ui_shadow_view.hpp"

void UIWindow::layout() {
    UIView::layout();
    set_size(inner_view->get_size());
    if(!style) set_style(UIWindowStyle::instance);
    header_view->set_fitting({size[0], header_height});
}

UIWindow::UIWindow(const Vec2f &position, const Vec2f &size, const char* title):
        UIView(position),
        header_view(new UIWindowHeaderView(this)) {
    append_child(new UIShadowView());
    append_child(inner_view);

    inner_shape = new RoundedRectShape({}, size, {18, 18, 18, 18});

    inner_view->append_child(header_view);
    inner_view->append_child(content_view);
    inner_view->set_shape(inner_shape);

    header_view->set_size({size[0], header_height});
    content_view->set_size(size);

    if(title) set_title(title);
}

void UIWindow::set_title(const char* string) {
    header_view->set_title(string);
}

void UIWindow::update_style() {
    if(!style) return;
    inner_view->set_fill_style(style->get_window_background_color());

    header_view->update_style();
}

void UIWindow::set_style(const UIWindowStyle* p_style) {
    style = p_style;
    update_style();
}

void UIWindow::set_active(bool p_is_active) {
    UIView::set_active(p_is_active);
    update_style();
}

void UIWindow::close() {
    WindowCloseEvent event { this };
    close_event_emitter.emit(&event);
    container->remove_window(this);
}

void UIWindow::set_size(const Vec2f &new_size) {
    UIView::set_size(new_size);

    inner_shape->resize(new_size);
    inner_view->set_shape(inner_shape);
}