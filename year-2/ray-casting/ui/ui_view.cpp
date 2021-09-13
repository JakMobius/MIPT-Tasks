#include "ui_view.hpp"
#include "point_test_iterator.hpp"

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
    PointTestIterator iterator(this, Vec2d { (double)event->x, (double)event->y });
    Vec2d internal_point;
    UIView* child = iterator.next(&internal_point);

    if(child) {
        MouseInEvent nested_event(internal_point.x, internal_point.y);

        child->on_mouse_in(&nested_event);
        current_hovered_child = child;
    }
}

void UIView::on_mouse_move(MouseMoveEvent* event) {
    PointTestIterator iterator(this, Vec2d { event->x, event->y });
    Vec2d internal_point;
    UIView* child = iterator.next(&internal_point);

    if(child && child == current_hovered_child) {
        double dx = child->transform.transform_x(event->dx, event->dy, 0);
        double dy = child->transform.transform_y(event->dx, event->dy, 0);
        MouseMoveEvent nested_event(internal_point.x, internal_point.y, dx, dy);
        child->on_mouse_move(&nested_event);
    } else {
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
}

void UIView::on_mouse_out(MouseOutEvent* event) {
    UIView* child = current_hovered_child;

    if(child) {
        Vec2d internal_point = Vec2d(event->x, event->y);

        MouseOutEvent nested_event(internal_point.x, internal_point.y);

        child->on_mouse_out(&nested_event);
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
    }
}

Vec2d UIView::get_local_position(const Vec2d &external_position) {
    Vec2d internal_point = external_position;
    internal_point -= position;
    internal_point.transform(get_inv_transform());
    return internal_point;
}
