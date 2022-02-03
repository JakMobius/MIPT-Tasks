//
// Created by Артем on 09.11.2021.
//

#include "ui_modal_view.hpp"
#include "ui_screen.hpp"

UIFillStyleColor UIModalViewOverlay::default_background {{ 0, 0, 0, 0.2 }};

void UIModalViewOverlay::on_mouse_click(MouseClickEvent* event) {
    UIView::on_mouse_click(event);
    if(!event->is_handled()) get_container()->close_modal(this);
    event->mark_handled();
}

void UIModalViewOverlay::set_stretched_child(UIView* view) {
    if(stretched_child) {
        if(stretched_child == view) return;
        int index = get_child_index(view);
        if(index >= 0) remove_child(index);
    }
    stretched_child = view;
    if(!view) return;
    append_child(view);
}

void UIModalViewOverlay::layout() {
    UIModalView::layout();
    stretched_child->set_position({});
    stretched_child->set_size(m_size);
}

void UIModalViewPresentModeOverlay::locate(UIModalContainerView* container, UIModalView* view) const {
    view->set_position({});
    view->set_size(container->get_size());
}

void UIModalViewPresentModeDropdown::locate(UIModalContainerView* container, UIModalView* view) const {
    Vec2f parent_position = parent->get_screen_position({0, 0});
    Vec2f parent_size = parent->get_screen_position(parent->get_size()) - parent_position;
    view->set_position(parent_position + Vec2f {0, parent_size[1]});
}
