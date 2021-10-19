//
// Created by Артем on 18.10.2021.
//

#include "ui_screen.hpp"

void UIScreen::present_modal(UISelectModalView* modal_view, UIView* parent) {
    Vec2f parent_position = parent->get_screen_position({0, 0});
    Vec2f parent_size = parent->get_screen_position(parent->get_size()) - parent_position;

    modal_view->set_position(parent_position + Vec2f {0, parent_size[1]});
    modal_view_container->append_child(modal_view);
    modal_view_container->present();
}
