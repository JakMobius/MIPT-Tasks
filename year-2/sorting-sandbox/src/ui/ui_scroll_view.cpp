//
// Created by Артем on 10.11.2021.
//

#include "ui_scroll_view.hpp"

void UIScrollView::layout() {
    UIView::layout();

    update_content_size();
}

void UIScrollView::on_mouse_scroll(MouseScrollEvent* event) {
    UIView::on_mouse_scroll(event);
    if(!event->is_handled()) {
        scroll_to(scroll_position - Vec2f {event->dx, event->dy});
        event->mark_handled();
    }
}

void UIScrollView::scroll_to(const Vec2f &position) {
    scroll_position = position;

    for(int axle = 0; axle < 2; axle++) {
        if(scroll_position[axle] + m_size[axle] > content_size[axle]) scroll_position.set(axle, content_size[axle] - m_size[axle]);
        if(scroll_position[axle] < 0) scroll_position.set(axle, 0);
    }

    content_view->set_position(-scroll_position);
}

UIScrollView::UIScrollView(const Vec2f &position, const Vec2f &size) : UIView(position, size) {
    set_masks_to_bounds(true);
    content_view = new UIView();
    append_child(content_view);
}

void UIScrollView::update_content_size() {
    float width = 0;
    float height = 0;

    auto children = content_view->get_children();

    for(int i = 0; i < children.size(); i++) {
        auto& child = children[i];
        Vec2f child_greatest_position = child->get_position();
        child_greatest_position += child->get_size();
        if(width < child_greatest_position[0]) width = child_greatest_position[0];
        if(height < child_greatest_position[1]) height = child_greatest_position[1];
    }

    content_size = { width, height };

    if(content_view->get_size() != content_size) {
        content_view->set_size(content_size);
    }

    Vec2f viewport_greatest_position(scroll_position);
    viewport_greatest_position += m_size;

    Vec2f scroll_adjustment {};

    for(int axle = 0; axle < 2; axle++) {
        if(viewport_greatest_position[axle] > content_size[axle]) {
            scroll_adjustment.set(axle, viewport_greatest_position[axle] - content_size[axle]);
        }
    }

    scroll_to(scroll_adjustment);
}
