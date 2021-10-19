//
// Created by Артем on 18.10.2021.
//

#include "ui_window.hpp"

const UIWindowStyle UI_WINDOW_DEFAULT_STYLE {
    {
        Vec4f {1, 0.5, 0.5, 1},
        Vec4f {1, 0.3, 0.3, 1},
        Vec4f {0.6, 0.2, 0.2, 1},
        UI_DEFAULT_BUTTON_STYLE.selected_color,
        UI_DEFAULT_BUTTON_STYLE.disabled_color,
        UI_DEFAULT_BUTTON_STYLE.inactive_color,
    },
    {
        Vec4f {0.5, 1, 0.5, 1},
        Vec4f {0.3, 1, 0.3, 1},
        Vec4f {0.2, 0.6, 0.2, 1},
        UI_DEFAULT_BUTTON_STYLE.selected_color,
        UI_DEFAULT_BUTTON_STYLE.disabled_color,
        UI_DEFAULT_BUTTON_STYLE.inactive_color,
    },
    Vec4f {1, 1, 1, 1},
    Vec4f {0.3, 0.5, 0.3, 1},
    Vec4f {0.3, 0.3, 0.3, 1},
    Vec4f {0.8, 0.8, 0.8, 1}
};

UIWindow::UIWindow(const Vec2f &position, const Vec2f &size, const char* title) : UIView(position, {}),
    window_size(size), header_view(new UIWindowHeaderView(this)) {
    append_child(header_view);
    append_child(content_view);

    if(title) set_title(title);
    set_style(&UI_WINDOW_DEFAULT_STYLE);
}

void UIWindow::layout() {
    UIView::layout();

    content_view->set_position({0, header_height});
    header_view->set_fitting({window_size[0], header_height});
    Vec2f new_size = window_size;
    new_size += content_view->get_position();
    set_size(new_size);
}

void UIWindow::set_title(const char* string) {
    header_view->set_title(string);
}

void UIWindow::set_style(const UIWindowStyle* p_style) {
    style = p_style;
    set_background(style->window_background_color);
    header_view->update_style();
}
