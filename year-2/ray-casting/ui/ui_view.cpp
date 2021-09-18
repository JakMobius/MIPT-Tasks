#include "ui_view.hpp"

void UIView::transform_context(DrawingContext* ctx) {
    ctx->transform.translate(position.x, position.y);
    ctx->transform.multiply(transform);
}

void UIView::draw(DrawingContext* ctx) {

    if(background.w > 0) {
        ctx->color = background.to_sf_color();
        ctx->draw_rect({0, 0}, size);
    }

    for(int i = 0; i < children.size; i++) {
        auto* child = children[i];
        Matrix3d saved_transform = ctx->transform;
        child->transform_context(ctx);
        child->draw(ctx);
        ctx->transform = saved_transform;
    }
}

void UIView::on_mouse_in(MouseInEvent* event) {
    Vec2d internal_point;
    UIView* child = test(Vec2d { event->x, event->y }, &internal_point);

    if(child) {
        MouseInEvent nested_event(internal_point.x, internal_point.y);

        child->on_mouse_in(&nested_event);
        current_hovered_child = child;
    }
}

UIView* UIView::test(const Vec2d& point, Vec2d* internal_point) const {
    for(int i = get_children().size - 1; i >= 0; i--) {
        UIView* child = get_children()[i];
        Vec2d absolute_point = point - child->get_position();
        absolute_point.transform(child->get_inv_transform());
        if(absolute_point.x < 0 || absolute_point.x >= child->get_size().x) continue;
        if(absolute_point.y < 0 || absolute_point.y >= child->get_size().y) continue;
        *internal_point = absolute_point;
        return child;
    }
    return nullptr;
}

void UIView::on_mouse_move(MouseMoveEvent* event) {
    Vec2d internal_point;
    UIView* child = nullptr;

    if(current_clicked_child) {
        child = current_clicked_child;
        internal_point = child->get_local_position(Vec2d { event->x, event->y });
    } else {
        child = test(Vec2d {event->x, event->y}, &internal_point);
    }

    if(child && child == current_hovered_child) {
        double dx = child->transform.transform_x(event->dx, event->dy, 0);
        double dy = child->transform.transform_y(event->dx, event->dy, 0);
        MouseMoveEvent nested_event(internal_point.x, internal_point.y, dx, dy);
        child->on_mouse_move(&nested_event);
    } else rehover(child, internal_point);
}

void UIView::on_mouse_out(MouseOutEvent* event) {

    if(current_hovered_child) {
        Vec2d internal_point = Vec2d(event->x, event->y);
        MouseOutEvent nested_event(internal_point.x, internal_point.y);
        current_hovered_child->on_mouse_out(&nested_event);
        current_hovered_child = nullptr;
    }

    if(current_clicked_child) {
        Vec2d internal_point = Vec2d(event->x, event->y);
        MouseUpEvent nested_event(internal_point.x, internal_point.y);
        current_clicked_child->on_mouse_up(&nested_event);
        current_clicked_child = nullptr;
    }
}

void UIView::on_mouse_down(MouseDownEvent* event) {
    current_clicked_child = current_hovered_child;
    if(current_clicked_child) {
        Vec2d internal_point = current_clicked_child->get_local_position({ event->x, event->y });
        MouseDownEvent nested_event(internal_point.x, internal_point.y);

        current_clicked_child->on_mouse_down(&nested_event);
    }
}

void UIView::on_mouse_up(MouseUpEvent* event) {
    if(current_clicked_child) {
        Vec2d internal_point = current_clicked_child->get_local_position({ event->x, event->y });
        MouseUpEvent nested_event(internal_point.x, internal_point.y);

        current_clicked_child->on_mouse_up(&nested_event);

        UIView* child = test(Vec2d {event->x, event->y}, &internal_point);
        if(child != current_hovered_child) rehover(child, internal_point);
        current_clicked_child = nullptr;
    }
}

Vec2d UIView::get_local_position(const Vec2d &external_position) {
    Vec2d internal_point = external_position;
    internal_point -= position;
    internal_point.transform(get_inv_transform());
    return internal_point;
}

void UIView::rehover(UIView* child, const Vec2d& internal_point) {
    if(child) {
        MouseInEvent nested_mouse_in_event(internal_point.x, internal_point.y);
        child->on_mouse_in(&nested_mouse_in_event);
    }
    if(current_hovered_child) {
        MouseOutEvent nested_mouse_out_event(internal_point.x, internal_point.y);
        current_hovered_child->on_mouse_out(&nested_mouse_out_event);
    }
    current_hovered_child = child;
}
