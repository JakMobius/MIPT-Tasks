//
// Created by Артем on 18.10.2021.
//

#include "ui_screen.hpp"

void UIScreen::present_modal(UIModalView* modal_view) {

    modal_view_container->present_modal(modal_view);
}
