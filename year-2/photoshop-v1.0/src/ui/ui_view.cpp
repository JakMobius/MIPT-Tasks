#include "ui_view.hpp"
#include "ui_screen.hpp"
#include "animations/ui_fill_style_color_animation.hpp"
#include "styles/fill_style/fill_style_texture.hpp"

const UIFillStyleColor UIViewWhiteBackground({1, 1, 1, 1});
const UIFillStyleColor UIViewRedBackground({1, 0, 0, 1});

void UIView::transform_context(DrawingContext* ctx) {
    ctx->transform.translate(position[0], position[1]);
    ctx->transform = transform.multiplied(ctx->transform);
}

void UIView::draw_self_and_children(DrawingContext* ctx) {
    draw(ctx);

    for(int i = 0; i < children.size(); i++) {
        auto* child = children[i];
        child->prepare_to_draw(ctx);
    }
}

void UIView::draw_self_texture_shaped(DrawingContext* ctx) {
    if(shape) {
        shape->draw(ctx);
    } else {
        ctx->fill_rect({}, size);
    }
}

void UIView::draw_in_texture(DrawingContext* ctx) {
    Matrix3f saved_transform = ctx->transform;

    if(!texture_valid) {
        ctx->push_render_target(texture);
        ctx->clear({0, 0, 0, 0});
        ctx->transform = {};
        draw_self_and_children(ctx);
        ctx->pop_render_target();
        texture_valid = true;
        ctx->transform = saved_transform;
    }

    transform_context(ctx);
    UIFillStyleTexture style { texture };
    ctx->set_fill_style(&style);
    draw_self_texture_shaped(ctx);
    ctx->transform = saved_transform;
}

void UIView::draw_without_texture(DrawingContext* ctx) {
    Matrix3f saved_transform = ctx->transform;
    transform_context(ctx);
    draw_self_and_children(ctx);
    needs_redraw = false;
    ctx->transform = saved_transform;
}

void UIView::prepare_to_draw(DrawingContext* ctx) {
    if(get_hidden()) return;
    if(needs_texture_decision) decide_whether_to_draw_to_texture();

    layout_if_needed();
    if(texture) draw_in_texture(ctx);
    else draw_without_texture(ctx);
}

void UIView::draw(DrawingContext* ctx) {
    if(fill_style) {
        ctx->set_fill_style(fill_style);
        ctx->fill_rect({0, 0}, size);
    }
}

void UIView::on_mouse_in(MouseInEvent* event) {
    Vec2f internal_point {};
    UIView* child = test(Vec2f { event->x, event->y }, &internal_point);

    hovered = true;

    if(child) {
        if(event->should_propagate()) {
            MouseInEvent nested_event(internal_point[0], internal_point[1]);
            child->on_mouse_in(&nested_event);
            if(nested_event.is_handled()) event->mark_handled();
        }
        current_hovered_child = child;
    }
}

UIView* UIView::test(const Vec2f& point, Vec2f* internal_point) const {
    for(int i = (int)children.size() - 1; i >= 0; i--) {
        UIView* child = children[i];
        if(!child->get_interactions_enabled()) continue;
        if(child->get_hidden()) continue;
        Vec2f absolute_point = point;
        absolute_point -= child->get_position();
        absolute_point *= child->get_inv_transform();
        if(absolute_point[0] < 0 || absolute_point[0] >= child->get_size()[0]) continue;
        if(absolute_point[1] < 0 || absolute_point[1] >= child->get_size()[1]) continue;
        *internal_point = absolute_point;
        return child;
    }
    return nullptr;
}

void UIView::on_mouse_move(MouseMoveEvent* event) {
    auto old_hovered_child = current_hovered_child;
    update_hovered_child(event->x, event->y);

    if(current_hovered_child == old_hovered_child && current_hovered_child) {
        auto internal_point = current_hovered_child->get_local_position({event->x, event->y});
        if(event->should_propagate()) {
            MouseMoveEvent nested_event(internal_point[0], internal_point[1]);
            current_hovered_child->on_mouse_move(&nested_event);
            if(nested_event.is_handled()) event->mark_handled();
        }
    }
}

void UIView::on_mouse_out(MouseOutEvent* event) {

    hovered = false;

    if(current_hovered_child) {
        Vec2f internal_point = Vec2f(event->x, event->y);
        if(event->should_propagate()) {
            MouseOutEvent nested_event(internal_point[0], internal_point[1]);
            current_hovered_child->on_mouse_out(&nested_event);
            if(nested_event.is_handled()) event->mark_handled();
        }
        current_hovered_child = nullptr;
    }

    if(current_clicked_child) {
        Vec2f internal_point = Vec2f(event->x, event->y);
        if(event->should_propagate()) {
            MouseUpEvent nested_event(internal_point[0], internal_point[1]);
            current_clicked_child->on_mouse_up(&nested_event);
            if(nested_event.is_handled()) event->mark_handled();
        }
        current_clicked_child = nullptr;
    }
}

void UIView::on_mouse_down(MouseDownEvent* event) {
    update_hovered_child(event->x, event->y);

    current_clicked_child = current_hovered_child;

    if(current_focused_child && current_clicked_child != current_focused_child) {
        current_focused_child->blur();
    }

    if(current_clicked_child) {
        if(!current_clicked_child->get_is_focused()) {
            current_clicked_child->focus();
        }

        Vec2f internal_point = current_clicked_child->get_local_position({ event->x, event->y });
        if(event->should_propagate()) {
            MouseDownEvent nested_event(internal_point[0], internal_point[1]);

            current_clicked_child->on_mouse_down(&nested_event);
            if(nested_event.is_handled()) {
                event->mark_handled();
                return;
            }
        }
    }

    clicked = true;
}

void UIView::on_mouse_up(MouseUpEvent* event) {
    update_hovered_child(event->x, event->y);

    bool handle_click = true;
    clicked = false;
    if(current_clicked_child) {
        Vec2f internal_point = current_clicked_child->get_local_position({ event->x, event->y });

        UIView* child = test(Vec2f {event->x, event->y}, &internal_point);
        if(child != current_hovered_child) {
            if(update_hover(child, internal_point)) event->mark_handled();
            handle_click = false;
        }

        if(event->should_propagate()) {
            MouseUpEvent nested_event(internal_point[0], internal_point[1]);
            current_clicked_child->on_mouse_up(&nested_event);
            if(nested_event.is_handled()) event->mark_handled();
        }
        current_clicked_child = nullptr;
    }

    if(handle_click && event->x >= 0 && event->y >= 0 && event->x < size[0] && event->y < size[1]) {
        auto click_event = MouseClickEvent(event->x, event->y);
        if(event->is_handled()) click_event.mark_handled();
        on_mouse_click(&click_event);
        if(click_event.is_handled()) event->mark_handled();
    }
}

Vec2f UIView::get_local_position(const Vec2f &external_position) {
    Vec2f internal_point = external_position;
    internal_point -= position;
    internal_point *= get_inv_transform();
    return internal_point;
}

Vec2f UIView::get_parent_position(const Vec2f &local_position) {
    Vec2f parent_point = local_position;
    parent_point *= get_transform();
    parent_point += position;
    return parent_point;
}

Vec2f UIView::get_screen_position(Vec2f local_position) {
    UIView* view = this;
    while(view) {
        local_position = view->get_parent_position(local_position);
        view = view->parent;
    }
    return local_position;
}

bool UIView::update_hover(UIView* child, const Vec2f& internal_point) {
    bool handled = false;
    if(child) {
        MouseInEvent nested_mouse_in_event(internal_point[0], internal_point[1]);
        child->on_mouse_in(&nested_mouse_in_event);
        handled |= nested_mouse_in_event.is_handled();
    }
    if(current_hovered_child) {
        MouseOutEvent nested_mouse_out_event(internal_point[0], internal_point[1]);
        current_hovered_child->on_mouse_out(&nested_mouse_out_event);
        handled |= nested_mouse_out_event.is_handled();
    }
    current_hovered_child = child;
    return handled;
}

void UIView::layout() {
    for(int i = 0; i < children.size(); i++) {
        children[i]->layout_if_needed();
    }
}

void UIView::append_child(UIView* child) {
    assert(!child->parent);
    children.push_back(child);
    child->parent = this;
    child->set_needs_layout();
}

void UIView::remove_child(int index) {
    auto child = children[index];

    if(child == current_hovered_child) current_hovered_child = nullptr;
    if(child == current_clicked_child) current_clicked_child = nullptr;
    if(child == current_focused_child) current_focused_child = nullptr;

    children.erase(children.begin() + index);
    child->parent = nullptr;
    set_needs_layout();
}

void UIView::layout_if_needed() {
    if(needs_layout || needs_children_layout) {
        layout();
        needs_layout = false;
        needs_children_layout = false;
    }
}

void UIView::set_needs_layout() {
    set_needs_redraw();
    needs_layout = true;
    if(parent) parent->set_needs_children_layout();
}

void UIView::set_needs_texture_update() {
    texture_valid = false;
}

void UIView::set_needs_redraw() {
    needs_redraw = true;
    set_needs_texture_update();
    if(parent) {
        parent->set_needs_texture_update();
        parent->set_needs_redraw();
    }
}

bool UIView::get_needs_redraw() const {
    return needs_redraw;
}

void UIView::set_fill_style(const UIFillStyle* p_fill_style, float animation_duration) {
    if(animation_duration > 0) {
        auto screen = get_screen();
        if(screen) {
            auto animation = new UIFillStyleAnimation(this, p_fill_style, animation_duration);
            auto* animation_controller = screen->get_animation_controller();
            animation_controller->add_animation(animation);
            return;
        }
    }

    fill_style = p_fill_style;
    set_needs_redraw();
}

void UIView::set_transform(const Matrix3f &new_transform) {
    transform = new_transform;
    inv_transform = transform.inverse();
    set_needs_redraw();
}

void UIView::set_size(const Vec2f &new_size) {
    size = new_size;
    set_needs_layout();
    if(texture) recreate_texture();
}

UIScreen* UIView::get_screen() {
    if(parent) return parent->get_screen();
    return nullptr;
}

void UIView::set_needs_children_layout() {
    needs_children_layout = true;
    if(parent) parent->set_needs_children_layout();
}

void UIView::on_mouse_click(MouseClickEvent* event) {

}

void UIView::on_mouse_scroll(MouseScrollEvent* event) {
    if(current_hovered_child && event->should_propagate()) {
        auto internal_point = current_hovered_child->get_local_position({event->x, event->y});
        MouseScrollEvent nested_event(internal_point[0], internal_point[1], event->dx, event->dy);
        current_hovered_child->on_mouse_scroll(&nested_event);
        if(nested_event.is_handled()) event->mark_handled();
    }
}

bool UIView::update_hovered_child(float x, float y) {
    Vec2f internal_point;
    UIView* child = nullptr;

    if(current_clicked_child) {
        child = current_clicked_child;
        internal_point = child->get_local_position(Vec2f { x, y });
    } else {
        child = test(Vec2f {x, y}, &internal_point);
    }

    if(!child || child != current_hovered_child) {
        return update_hover(child, internal_point);
    }
    return false;
}

int UIView::get_child_index(UIView* child) {
    for(int i = 0; i < children.size(); i++) {
        if(children[i] == child) return i;
    }
    return -1;
}

UIView::~UIView() {
    while(!children.empty()) {
        auto* child = children[children.size() - 1];
        children.pop_back();
        child->destroy();
    }
    delete texture;
}

void UIView::decide_whether_to_draw_to_texture() {
    needs_texture_decision = false;
    set_draw_to_texture(shape || masks_to_bounds);
}

void UIView::set_draw_to_texture(bool use_texture) {
    if(use_texture && !texture) {
        recreate_texture();
        texture_valid = false;
    } else if(!use_texture && texture) {
        delete texture;
        texture = nullptr;
    }
}

void UIView::recreate_texture() {
    delete texture;
    texture = new DrawingTargetTexture((Vec2i)size);
}

Shape* UIView::get_shape() const {
    return shape;
}

void UIView::set_shape(Shape* p_shape) {
    shape = p_shape;
    needs_texture_decision = true;
}

void UIView::on_key_down(KeyDownEvent* event) {
    if(current_focused_child) {
        if(event->should_propagate()) {
            current_focused_child->on_key_down(event);
        }
    } else if(focused && event->code == KeyCode::tab) {
        if(event->shift) focus_previous();
        else focus_next();
        event->mark_handled();
    }
}

void UIView::on_key_up(KeyUpEvent* event) {
    if(current_focused_child) {
        if(event->should_propagate()) {
            current_focused_child->on_key_up(event);
        }
    }
}

void UIView::on_text_enter(TextEnterEvent* event) {
    if(current_focused_child) {
        if(event->should_propagate()) {
            current_focused_child->on_text_enter(event);
        }
    }
}

void UIView::focus() {
    if(focused) return;
    focused = true;
    if(parent) parent->focus_child(this);
}

void UIView::blur() {
    if(!focused) return;
    focused = false;
    if(current_focused_child) current_focused_child->blur();
    if(parent) parent->handle_child_blur();
}

void UIView::focus_child(UIView* child) {
    if(current_focused_child && current_focused_child != child) current_focused_child->blur();
    focus();
    current_focused_child = child;
}

bool UIView::focus_next() {
    return focus_first_child() || focus_next_upwards();
}

bool UIView::focus_previous() {
    return focus_last_child() || focus_previous_upwards();
}

bool UIView::focus_next_upwards() {
    if(!parent) return false;

    int index = parent->get_child_index(this);
    if(index < 0) return false;

    if(!parent->focus_first_child(index + 1)) return parent->focus_next_upwards();
    return false;
}

bool UIView::focus_previous_upwards() {
    if(!parent) return false;

    int index = parent->get_child_index(this);
    if(index < 0) return false;

    if(!parent->focus_last_child(index)) return parent->focus_previous_upwards();
    return false;
}

bool UIView::focus_first_child(int start_index, int end_index) {
    if(start_index < 0) start_index = 0;
    if(end_index > children.size()) end_index = children.size();

    for(int i = start_index; i < end_index; i++) {
        auto child = children[i];

        if(child->focusable) {
            child->focus();
            return true;
        }

        if(child->focus_first_child()) return true;
    }
    if(wrap_focus && start_index != 0) {
        return focus_first_child(0, start_index);
    }
    return false;
}

bool UIView::focus_last_child(int start_index, int end_index) {
    if(start_index > (int)children.size()) start_index = children.size();
    if(end_index < 0) end_index = 0;

    for(int i = start_index - 1; i >= end_index; i--) {
        auto child = children[i];
        if(child->focusable && child != current_focused_child) {
            child->focus();
            return true;
        }

        if(child->focus_last_child()) return true;
    }

    if(wrap_focus && start_index != children.size()) {
        return focus_last_child(children.size(), start_index);
    }

    return false;
}

void UIView::set_active(bool p_is_active) {
    active = p_is_active;
    for(int i = 0; i < children.size(); i++) {
        children[i]->set_active(p_is_active);
    }
}

void UIView::handle_child_blur() {
    current_focused_child = nullptr;
    blur();
}

void UIView::destroy() {
    if(destroyed) return;
    destroyed = true;
    ViewDestroyEvent event { this };
    destroy_event_emitter.emit(&event);
    delete this;
}
