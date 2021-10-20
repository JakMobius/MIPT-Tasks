
#include "ui_select.hpp"
#include "../ui_screen.hpp"

void UISelectView::on_mouse_click(MouseClickEvent* event) {
    auto* screen = get_screen();

    if(!screen) return;

    event->mark_handled();

    auto* modal = new UISelectModalView(&options);

    screen->present_modal(modal, this);
}

void UISelectView::draw(UIDrawingContext* ctx) {
    UIView::draw(ctx);

    ctx->set_color({0, 0, 0, 1});

    Vec2f arrow_position = get_size() + Vec2f {-20, 0};
    arrow_position.set_y(arrow_position[1] / 2);
    ctx->draw_line(arrow_position + Vec2f {-10, -5}, arrow_position + Vec2f {0, 5}, 3);
    ctx->draw_line(arrow_position + Vec2f {10, -5}, arrow_position + Vec2f {0, 5}, 3);
}
