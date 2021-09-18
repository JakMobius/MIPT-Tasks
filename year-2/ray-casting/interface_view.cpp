#include "interface_view.hpp"
#include "utils/vec2.hpp"

InterfaceView::InterfaceView(const Vec2d &position): UIView(position, {400, 120}) {
    set_background({0, 0, 0, 0});

    append_child(&slider);
    append_child(&animation_button);

    animation_button.set_title("Toggle animation");
}
