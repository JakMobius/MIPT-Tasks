//
// Created by Артем on 18.10.2021.
//

#include "ui_window_header.hpp"
#include "./ui_window.hpp"
#include "../../utils/dispatch_queue.hpp"
#include "../ui_shadow_view.hpp"

void UIWindowHeaderView::on_mouse_move(MouseMoveEvent* event) {
    UIStackView::on_mouse_move(event);
    if(clicked && window) {
        Vec2f position = window->get_position();
        position.set_x(position[0] + event->x - drag_start_position[0]);
        position.set_y(position[1] + event->y - drag_start_position[1]);
        window->set_position(position);
    }
}

UIWindowHeaderView::UIWindowHeaderView(UIWindow* window) : UIStackView(UIStackViewDirection::x, position), window(window) {
    set_primary_alignment(UIStackViewPrimaryAlignment::space_between);
    set_lateral_alignment(UIStackViewLateralAlignment::center);
    setup_buttons_container();

    header->get_text_drawer()->set_v_alignment(VTextAlignmentCenter);
    header->get_text_drawer()->set_h_alignment(HTextAlignmentCenter);

    append_child(buttons_container);
    append_child(header);
    append_child(spacer);
}

void UIWindowHeaderView::setup_buttons_container() {
    buttons_container->set_lateral_alignment(UIStackViewLateralAlignment::center);
    buttons_container->set_fitting({});
    buttons_container->set_item_spacing(5);
    buttons_container->set_insets({5});

    buttons_container->append_child(close_button);
//    buttons_container->append_child(fullscreen_button);

    close_button->set_callback([this]() {
        // Close window asynchronously, so
        // interface hierarchy does not get
        // changed until event propagation is
        // done.

        DispatchQueue::main.push([&]() {
            window->close();
        });
    });
}

void UIWindowHeaderView::update_style() {
    if(window->get_active()) {
        set_fill_style(window->get_style()->get_header_background_color());
    } else {
        set_fill_style(window->get_style()->get_inactive_header_background_color());
    }
    header->get_text_drawer()->set_font_color(window->get_style()->get_window_header_color());
    close_button->set_style(window->get_style()->get_close_button_style());
    fullscreen_button->set_style(window->get_style()->get_fullscreen_button_style());
}

void UIWindowHeaderView::on_mouse_down(MouseDownEvent* event) {
    UIStackView::on_mouse_down(event);
    drag_start_position = { event->x, event->y };
}

void UIWindowHeaderView::layout() {
    buttons_container->set_fitting({{}, 42});
    buttons_container->set_insets({0, 7});
    buttons_container->layout_if_needed();
    spacer->set_size(buttons_container->get_size());
    UIStackView::layout();
}
