
#include "ui_select.hpp"
#include "../ui_screen.hpp"

void UISelectView::on_mouse_click(MouseClickEvent* event) {
    auto* screen = get_screen();

    if(!screen) return;

    event->mark_handled();

    auto* present_mode = new UIModalViewPresentModeDropdown(this);
    auto* modal = new UISelectModalView(present_mode, &options);

    modal->set_style(style);

    screen->present_modal(modal);
}

void UISelectView::draw(DrawingContext* ctx) {
    UIView::draw(ctx);

    ctx->set_stroke_style(style->get_arrow_style());

    Vec2f arrow_position = get_size() + Vec2f {-20, 0};
    arrow_position.set_y(arrow_position[1] / 2);
    ctx->stroke_line(arrow_position + Vec2f {-10, -5}, arrow_position + Vec2f {0, 5}, 3);
    ctx->stroke_line(arrow_position + Vec2f {10, -5}, arrow_position + Vec2f {0, 5}, 3);
}

void UISelectView::set_style(const UISelectViewStyle* p_style) {
    Styled::set_style(p_style);
    set_fill_style(style->get_background_color());
}
