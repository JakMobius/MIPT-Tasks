//
// Created by Артем on 18.10.2021.
//

#include "ui_modal_container.hpp"

void UIModalContainerView::close_single_modal(int index) {
    auto* modal = modal_views_stack[index];
    auto child_index = get_child_index(modal);
    if(child_index >= 0) remove_child(child_index);
    modal->destroy();
}

void UIModalContainerView::on_mouse_click(MouseClickEvent* event) {
    UIView::on_mouse_click(event);

    if(!event->is_handled()) {
        hide();
    }
}

void UIModalContainerView::close_modal(UIModalView* view) {
    for(int i = modal_views_stack.size() - 1; i >= 0; i--) {
        close_single_modal(i);
        if(modal_views_stack[i] == view) {
            modal_views_stack.pop_back();
            break;
        }
    }
    if(modal_views_stack.empty()) {
        set_hidden(true);
    }
}

void UIModalContainerView::hide() {
    close_modal(nullptr);
    set_hidden(true);
}

void UIModalContainerView::present_modal(UIModalView* modal_view) {
    modal_view->set_container(this);
    append_child(modal_view);
    modal_view->get_present_mode()->locate(this, modal_view);
    modal_views_stack.push_back(modal_view);
    present();
}
