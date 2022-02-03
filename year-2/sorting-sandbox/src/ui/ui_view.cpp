#include "ui_view.hpp"
#include "ui_screen.hpp"
#include "animations/ui_fill_style_color_animation.hpp"
#include "styles/fill_style/fill_style_texture.hpp"

const UIFillStyleColor UIViewWhiteBackground({1, 1, 1, 1});
const UIFillStyleColor UIViewRedBackground({1, 0, 0, 1});

void UIView::transform_context(DrawingContext* ctx) {
    ctx->transform.translate(m_position[0], m_position[1]);
    ctx->transform = m_transform.multiplied(ctx->transform);
}

void UIView::draw_self_and_children(DrawingContext* ctx) {
    draw(ctx);

    for(int i = 0; i < m_children.size(); i++) {
        auto* child = m_children[i];
        child->prepare_to_draw(ctx);
    }
}

void UIView::draw_self_texture_shaped(DrawingContext* ctx) {
    if(m_shape) {
        m_shape->draw(ctx);
    } else {
        ctx->fill_rect({}, m_size);
    }
}

void UIView::draw_in_texture(DrawingContext* ctx) {
    Matrix3f saved_transform = ctx->transform;

    if(!m_texture_valid || !m_cache_texture) {
        ctx->push_render_target(m_texture);
        ctx->clear({0, 0, 0, 0});
        ctx->transform = {};
        draw_self_and_children(ctx);
        ctx->pop_render_target();
        m_texture_valid = true;
        ctx->transform = saved_transform;
    }

    transform_context(ctx);
    UIFillStyleTexture style {m_texture };
    ctx->set_fill_style(&style);
    draw_self_texture_shaped(ctx);
    ctx->transform = saved_transform;
}

void UIView::draw_without_texture(DrawingContext* ctx) {
    Matrix3f saved_transform = ctx->transform;
    transform_context(ctx);

    draw_self_and_children(ctx);
    ctx->transform = saved_transform;
}

void UIView::prepare_to_draw(DrawingContext* ctx) {
    if(get_hidden()) return;
    if(m_needs_texture_decision) decide_whether_to_draw_to_texture();

    layout_if_needed();
    m_needs_redraw = false;
    if(m_texture) draw_in_texture(ctx);
    else draw_without_texture(ctx);
}

void UIView::draw(DrawingContext* ctx) {
    if(m_fill_style) {
        ctx->set_fill_style(m_fill_style);
        ctx->fill_rect({0, 0}, m_size);
    }
}

void UIView::on_mouse_in(MouseInEvent* event) {
    Vec2f internal_point {};
    UIView* child = test(Vec2f { event->x, event->y }, &internal_point);

    m_hovered = true;

    if(child) {
        if(event->should_propagate()) {
            MouseInEvent nested_event(internal_point[0], internal_point[1]);
            nested_event.m_modifiers = event->m_modifiers;
            child->on_mouse_in(&nested_event);
            if(nested_event.is_handled()) event->mark_handled();
        }
        m_current_hovered_child = child;
    }
}

UIView* UIView::test(const Vec2f& point, Vec2f* internal_point) const {
    for(int i = (int)m_children.size() - 1; i >= 0; i--) {
        UIView* child = m_children[i];
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
    auto old_hovered_child = m_current_hovered_child;
    update_hovered_child(event->x, event->y);

    if(m_current_hovered_child == old_hovered_child && m_current_hovered_child) {
        auto internal_point = m_current_hovered_child->get_local_position({event->x, event->y});
        if(event->should_propagate()) {
            MouseMoveEvent nested_event(internal_point[0], internal_point[1]);
            nested_event.m_modifiers = event->m_modifiers;
            m_current_hovered_child->on_mouse_move(&nested_event);
            if(nested_event.is_handled()) event->mark_handled();
        }
    }
}

void UIView::on_mouse_out(MouseOutEvent* event) {

    m_hovered = false;

    if(m_current_hovered_child) {
        Vec2f internal_point = Vec2f(event->x, event->y);
        if(event->should_propagate()) {
            MouseOutEvent nested_event(internal_point[0], internal_point[1]);
            nested_event.m_modifiers = event->m_modifiers;
            m_current_hovered_child->on_mouse_out(&nested_event);
            if(nested_event.is_handled()) event->mark_handled();
        }
        m_current_hovered_child = nullptr;
    }

    if(m_current_clicked_child) {
        Vec2f internal_point = Vec2f(event->x, event->y);
        if(event->should_propagate()) {
            MouseUpEvent nested_event(internal_point[0], internal_point[1]);
            nested_event.m_modifiers = event->m_modifiers;
            m_current_clicked_child->on_mouse_up(&nested_event);
            if(nested_event.is_handled()) event->mark_handled();
        }
        m_current_clicked_child = nullptr;
    }
}

void UIView::on_mouse_down(MouseDownEvent* event) {
    update_hovered_child(event->x, event->y);

    m_current_clicked_child = m_current_hovered_child;

    if(m_current_focused_child && m_current_clicked_child != m_current_focused_child) {
        m_current_focused_child->blur();
    }

    if(m_current_clicked_child) {
        if(!m_current_clicked_child->get_is_focused()) {
            m_current_clicked_child->focus();
        }

        Vec2f internal_point = m_current_clicked_child->get_local_position({event->x, event->y });
        if(event->should_propagate()) {
            MouseDownEvent nested_event(internal_point[0], internal_point[1]);
            nested_event.m_modifiers = event->m_modifiers;

            m_current_clicked_child->on_mouse_down(&nested_event);
            if(nested_event.is_handled()) {
                event->mark_handled();
                return;
            }
        }
    }

    m_clicked = true;
}

void UIView::on_mouse_up(MouseUpEvent* event) {
    update_hovered_child(event->x, event->y);

    bool handle_click = true;
    m_clicked = false;
    if(m_current_clicked_child) {
        Vec2f internal_point = m_current_clicked_child->get_local_position({event->x, event->y });

        UIView* child = test(Vec2f {event->x, event->y}, &internal_point);
        if(child != m_current_hovered_child) {
            if(update_hover(child, internal_point)) event->mark_handled();
            handle_click = false;
        }

        if(event->should_propagate()) {
            MouseUpEvent nested_event(internal_point[0], internal_point[1]);
            nested_event.m_modifiers = event->m_modifiers;
            m_current_clicked_child->on_mouse_up(&nested_event);
            if(nested_event.is_handled()) event->mark_handled();
        }
        m_current_clicked_child = nullptr;
    }

    if(handle_click && event->x >= 0 && event->y >= 0 && event->x < m_size[0] && event->y < m_size[1]) {
        auto click_event = MouseClickEvent(event->x, event->y);
        if(event->is_handled()) click_event.mark_handled();
        on_mouse_click(&click_event);
        if(click_event.is_handled()) event->mark_handled();
    }
}

Vec2f UIView::get_local_position(const Vec2f &external_position) {
    Vec2f internal_point = external_position;
    internal_point -= m_position;
    internal_point *= get_inv_transform();
    return internal_point;
}

Vec2f UIView::get_parent_position(const Vec2f &local_position) {
    Vec2f parent_point = local_position;
    parent_point *= get_transform();
    parent_point += m_position;
    return parent_point;
}

Vec2f UIView::get_screen_position(Vec2f local_position) {
    UIView* view = this;
    while(view) {
        local_position = view->get_parent_position(local_position);
        view = view->m_parent;
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
    if(m_current_hovered_child) {
        MouseOutEvent nested_mouse_out_event(internal_point[0], internal_point[1]);
        m_current_hovered_child->on_mouse_out(&nested_mouse_out_event);
        handled |= nested_mouse_out_event.is_handled();
    }
    m_current_hovered_child = child;
    return handled;
}

void UIView::layout() {
    for(int i = 0; i < m_children.size(); i++) {
        m_children[i]->layout_if_needed();
    }
}

void UIView::append_child(UIView* child) {
    assert(!child->m_parent);
    m_children.push_back(child);
    child->m_parent = this;
    child->set_needs_layout();
}

void UIView::remove_child(int index) {
    auto child = m_children[index];

    if(child == m_current_hovered_child) m_current_hovered_child = nullptr;
    if(child == m_current_clicked_child) m_current_clicked_child = nullptr;
    if(child == m_current_focused_child) m_current_focused_child = nullptr;

    m_children.erase(m_children.begin() + index);
    child->m_parent = nullptr;
    set_needs_layout();
}

void UIView::layout_if_needed() {
    if(m_needs_layout || m_needs_children_layout) {
        layout();
        m_needs_layout = false;
        m_needs_children_layout = false;
    }
}

void UIView::set_needs_layout() {
    set_needs_redraw();
    m_needs_layout = true;
    if(m_parent) m_parent->set_needs_children_layout();
}

void UIView::set_needs_texture_update() {
    m_texture_valid = false;
}

void UIView::set_needs_redraw() {
    if(m_needs_redraw) return;
    m_needs_redraw = true;
    set_needs_texture_update();
    if(m_parent) {
        m_parent->set_needs_texture_update();
        m_parent->set_needs_redraw();
    }
}

bool UIView::get_needs_redraw() const {
    return m_needs_redraw;
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

    m_fill_style = p_fill_style;
    set_needs_redraw();
}

void UIView::set_transform(const Matrix3f &new_transform) {
    m_transform = new_transform;
    m_inv_transform = m_transform.inverse();
    set_needs_redraw();
}

void UIView::set_size(const Vec2f &new_size) {
    if(m_size == new_size) return;
    m_size = new_size;
    set_needs_layout();
    if(m_texture) recreate_texture();
}

UIScreen* UIView::get_screen() {
    if(m_parent) return m_parent->get_screen();
    return nullptr;
}

void UIView::set_needs_children_layout() {
    m_needs_children_layout = true;
    if(m_parent) m_parent->set_needs_children_layout();
}

void UIView::on_mouse_click(MouseClickEvent* event) {

}

void UIView::on_mouse_scroll(MouseScrollEvent* event) {
    if(m_current_hovered_child && event->should_propagate()) {
        auto internal_point = m_current_hovered_child->get_local_position({event->x, event->y});
        MouseScrollEvent nested_event(internal_point[0], internal_point[1], event->dx, event->dy);
        nested_event.m_modifiers = event->m_modifiers;

        m_current_hovered_child->on_mouse_scroll(&nested_event);
        if(nested_event.is_handled()) event->mark_handled();
    }
}

bool UIView::update_hovered_child(float x, float y) {
    Vec2f internal_point;
    UIView* child = nullptr;

    if(m_current_clicked_child) {
        child = m_current_clicked_child;
        internal_point = child->get_local_position(Vec2f { x, y });
    } else {
        child = test(Vec2f {x, y}, &internal_point);
    }

    if(!child || child != m_current_hovered_child) {
        return update_hover(child, internal_point);
    }
    return false;
}

int UIView::get_child_index(UIView* child) {
    for(int i = 0; i < m_children.size(); i++) {
        if(m_children[i] == child) return i;
    }
    return -1;
}

UIView::~UIView() {
    while(!m_children.empty()) {
        auto* child = m_children[m_children.size() - 1];
        m_children.pop_back();
        child->destroy();
    }
    delete m_texture;
}

void UIView::decide_whether_to_draw_to_texture() {
    m_needs_texture_decision = false;
    set_draw_to_texture(m_shape || m_masks_to_bounds);
}

void UIView::set_draw_to_texture(bool use_texture) {
    if(use_texture && !m_texture) {
        recreate_texture();
        m_texture_valid = false;
    } else if(!use_texture && m_texture) {
        delete m_texture;
        m_texture = nullptr;
    }
}

void UIView::recreate_texture() {
    delete m_texture;
    m_texture = new DrawingTargetTexture((Vec2i)m_size);
}

Shape* UIView::get_shape() const {
    return m_shape;
}

void UIView::set_shape(Shape* p_shape) {
    m_shape = p_shape;
    m_needs_texture_decision = true;
}

void UIView::on_key_down(KeyDownEvent* event) {
    if(m_current_focused_child) {
        if(event->should_propagate()) {
            m_current_focused_child->on_key_down(event);
        }
    } else if(m_focused && event->code == KeyCode::tab) {
        if(event->m_modifiers.shift) focus_previous();
        else focus_next();
        event->mark_handled();
    }
}

void UIView::on_key_up(KeyUpEvent* event) {
    if(m_current_focused_child) {
        if(event->should_propagate()) {
            m_current_focused_child->on_key_up(event);
        }
    }
}

void UIView::on_text_enter(TextEnterEvent* event) {
    if(m_current_focused_child) {
        if(event->should_propagate()) {
            m_current_focused_child->on_text_enter(event);
        }
    }
}

void UIView::focus() {
    if(m_focused) return;
    m_focused = true;
    if(m_parent) m_parent->focus_child(this);
}

void UIView::blur() {
    if(!m_focused) return;
    m_focused = false;
    if(m_current_focused_child) m_current_focused_child->blur();
    if(m_parent) m_parent->handle_child_blur();
}

void UIView::focus_child(UIView* child) {
    if(m_current_focused_child && m_current_focused_child != child) m_current_focused_child->blur();
    focus();
    m_current_focused_child = child;
}

bool UIView::focus_next() {
    return focus_first_child() || focus_next_upwards();
}

bool UIView::focus_previous() {
    return focus_last_child() || focus_previous_upwards();
}

bool UIView::focus_next_upwards() {
    if(!m_parent) return false;

    int index = m_parent->get_child_index(this);
    if(index < 0) return false;

    if(!m_parent->focus_first_child(index + 1)) return m_parent->focus_next_upwards();
    return false;
}

bool UIView::focus_previous_upwards() {
    if(!m_parent) return false;

    int index = m_parent->get_child_index(this);
    if(index < 0) return false;

    if(!m_parent->focus_last_child(index)) return m_parent->focus_previous_upwards();
    return false;
}

bool UIView::focus_first_child(int start_index, int end_index) {
    if(start_index < 0) start_index = 0;
    if(end_index > m_children.size()) end_index = m_children.size();

    for(int i = start_index; i < end_index; i++) {
        auto child = m_children[i];

        if(child->m_focusable) {
            child->focus();
            return true;
        }

        if(child->focus_first_child()) return true;
    }
    if(m_wrap_focus && start_index != 0) {
        return focus_first_child(0, start_index);
    }
    return false;
}

bool UIView::focus_last_child(int start_index, int end_index) {
    if(start_index > (int)m_children.size()) start_index = m_children.size();
    if(end_index < 0) end_index = 0;

    for(int i = start_index - 1; i >= end_index; i--) {
        auto child = m_children[i];
        if(child->m_focusable && child != m_current_focused_child) {
            child->focus();
            return true;
        }

        if(child->focus_last_child()) return true;
    }

    if(m_wrap_focus && start_index != m_children.size()) {
        return focus_last_child(m_children.size(), start_index);
    }

    return false;
}

void UIView::set_active(bool p_is_active) {
    m_active = p_is_active;
    for(int i = 0; i < m_children.size(); i++) {
        m_children[i]->set_active(p_is_active);
    }
}

void UIView::handle_child_blur() {
    m_current_focused_child = nullptr;
    blur();
}

void UIView::destroy() {
    if(m_destroyed) return;
    m_destroyed = true;
    ViewDestroyEvent event { this };
    m_destroy_event_emitter.emit(&event);
    delete this;
}
