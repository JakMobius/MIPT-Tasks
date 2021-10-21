//
// Created by Артем on 18.10.2021.
//

#include "ui_window.hpp"
#include "../../utils/dispatch_queue.hpp"

void UIWindow::layout() {
    UIStackView::layout();
    header_view->set_fitting({size[0], header_height});
}

UIWindow::UIWindow(const Vec2f &position, const Vec2f &size, const char* title):
        UIStackView(UIStackViewDirection::y, position),
        header_view(new UIWindowHeaderView(this)) {
    append_child(header_view);
    append_child(content_view);

    header_view->set_size({size[0], header_height});
    content_view->set_size(size);

    if(title) set_title(title);
    set_style(UIWindowStyle::instance);
}

void UIWindow::set_title(const char* string) {
    header_view->set_title(string);
}

void UIWindow::update_style() {
    set_fill_style(style->get_window_background_color());
    header_view->update_style();
}

void UIWindow::set_style(const UIWindowStyle* p_style) {
    style = p_style;
    update_style();
}

void UIWindow::set_active(bool p_is_active) {
    UIStackView::set_active(p_is_active);
    update_style();
}

void UIWindow::close() {
    DispatchQueue::main.push([&]() {
        container->remove_window(this);
    });
}
