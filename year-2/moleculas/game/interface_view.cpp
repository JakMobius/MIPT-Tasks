#include "interface_view.hpp"


InterfaceView::InterfaceView(const Vec2d &position): UIView(position, {400, 120}) {
    set_background({0, 0, 0, 0});

    append_child(&slider);
    append_child(&reaction_button);
}

void InterfaceView::update_reactions_button(bool reactions_on) {
    if(reactions_on) {
        reaction_button.set_title("Disable reactions");
    } else {
        reaction_button.set_title("Enable reactions");
    }
}
