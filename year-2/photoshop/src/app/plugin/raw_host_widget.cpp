//
// Created by Артем on 14.12.2021.
//

#include "raw_host_widget.hpp"
#include "host_render_target.hpp"
#include "app_interface.hpp"

void RawHostWidget::set_position(const PUPPY::Vec2f &position_) {
    view->set_position({position_.x, position_.y});
}

void RawHostWidget::set_size(const PUPPY::Vec2f &size_) {
    view->set_size({size_.x, size_.y});
}

PUPPY::WBody RawHostWidget::get_body() {
    auto size = view->get_size();
    auto position = view->get_position();

    body.size.x = size[0];
    body.size.y = size[1];
    body.position.x = position[0];
    body.position.y = position[1];
    return body;
}

void RawHostWidget::set_body(const PUPPY::WBody &body_) {
    body = body_;
}

PUPPY::Widget* RawHostWidget::get_parent() const {
    return parent;
}

void RawHostWidget::set_parent(PUPPY::Widget* parent_) {
    parent = dynamic_cast<RawHostWidget*>(parent_);
}

PUPPY::RenderTarget* RawHostWidget::get_texture() {
    return render_target;
}

void RawHostWidget::set_texture(PUPPY::RenderTarget* texture_) {
    render_target = texture_;
}

bool RawHostWidget::is_active() {
    if(!view) return false;
    return view->get_is_focused();
}

bool RawHostWidget::is_inside(const PUPPY::Vec2f &pos) {
    auto real_body = get_body();
    return (pos.x >= 0) && (pos.y >= 0) && (pos.x < real_body.size.x) && (pos.y < real_body.size.y);
}

bool RawHostWidget::add_child(PUPPY::Widget* plugin_child) {
    if(!view) return false;
    RawHostWidget* child = dynamic_cast<RawHostWidget*>(plugin_child);
    if(!child) {
        children.push_back(plugin_child);
        return true;
    }
    plugin_child->set_parent(this);
    view->append_child(child->get_view());
    return true;
}

void RawHostWidget::set_to_delete() {
    view->destroy();
    delete this;
}

bool RawHostWidget::delete_child(PUPPY::Widget* plugin_child) {
    printf("delete_child is not implemented");
    return false;
}

bool RawHostWidget::delete_from_parent() {
    printf("delete_from_parent is not implemented");
    return false;
}

void RawHostWidget::on_render(const PUPPY::Event::Render &event) {

}

void RawHostWidget::on_tick(const PUPPY::Event::Tick &event) {
    for(auto& child : children) child->on_tick(event);
}

void RawHostWidget::on_mouse_press(const PUPPY::Event::MousePress &event) {
    for(int i = children.size() - 1; i >= 0; i--) {
        auto child = children[i];
        auto child_body = child->get_body();

        PUPPY::Vec2f new_pos = event.position;
        new_pos.x -= child_body.position.x;
        new_pos.y -= child_body.position.y;
        if(!child->is_inside(new_pos)) continue;
        PUPPY::Event::MousePress new_event { new_pos, event.button };

        child->on_mouse_press(new_event);
        captured_child = child;
        break;
    }
}

void RawHostWidget::on_mouse_release(const PUPPY::Event::MouseRelease &event) {
    PUPPY::Event::MouseRelease new_event { event.position, event.button };

    if(!captured_child) return;
    auto child_body = captured_child->get_body();
    new_event.position.x -= child_body.position.x;
    new_event.position.y -= child_body.position.y;
    captured_child->on_mouse_release(new_event);
    captured_child = nullptr;
}

void RawHostWidget::on_mouse_move(const PUPPY::Event::MouseMove &event) {
    PUPPY::Event::MouseMove new_event { event.from, event.to };
    if(!captured_child) {
        for(int i = children.size() - 1; i >= 0; i--) {
            auto child = children[i];
            auto child_body = child->get_body();

            PUPPY::Vec2f new_from_pos = event.from;
            PUPPY::Vec2f new_to_pos = event.from;
            new_from_pos.x -= child_body.position.x;
            new_from_pos.y -= child_body.position.y;
            new_to_pos.x -= child_body.position.x;
            new_to_pos.y -= child_body.position.y;
            if(!child->is_inside(new_from_pos)) continue;
            PUPPY::Event::MouseMove new_event { new_from_pos, new_to_pos };

            child->on_mouse_move(new_event);
            break;
        }
        return;
    }

    auto child_body = captured_child->get_body();
    new_event.from.x -= child_body.position.x;
    new_event.from.y -= child_body.position.y;
    new_event.to.x -= child_body.position.x;
    new_event.to.y -= child_body.position.y;
    captured_child->on_mouse_move(new_event);
}

void RawHostWidget::on_key_down(const PUPPY::Event::KeyDown &event) {
    for(auto& child : children) {
        if(child->is_active()) child->on_key_down(event);
    }
}

void RawHostWidget::on_key_up(const PUPPY::Event::KeyUp &event) {
    for(auto& child : children) {
        if(child->is_active()) child->on_key_up(event);
    }

}

void RawHostWidget::on_text_enter(const PUPPY::Event::TextEnter &event) {
    for(auto& child : children) {
        if(child->is_active()) child->on_text_enter(event);
    }
}

void RawHostWidget::on_scroll(const PUPPY::Event::Scroll &event) {
    for(auto& child : children) child->on_scroll(event);
}

void RawHostWidget::on_hide(const PUPPY::Event::Hide &event) {
    for(auto& child : children) child->on_hide(event);
}

void RawHostWidget::on_show(const PUPPY::Event::Show &event) {
    for(auto& child : children) child->on_show(event);
}

void RawHostWidget::hide() {
    if(!view) return;
    view->set_hidden(true);
    PUPPY::Event::Hide widget_event {};
    on_hide(widget_event);
}

void RawHostWidget::show() {
    if(!view) return;
    view->set_hidden(false);
    PUPPY::Event::Show widget_event {};
    on_show(widget_event);
}

void RawHostWidget::focus() {
    if(!view) return;
    view->focus();
}

void RawHostWidget::set_caption(const char* text, size_t font_size, const PUPPY::Vec2f* pos) {

}

void RawHostWidget::set_base_color(const PUPPY::RGBA &color) {

}

void RawHostWidget::set_view(UIView* p_view) {
    view = p_view;

    EventHandler<ViewDestroyEvent>* handler = new EventHandler<ViewDestroyEvent>;
    *handler = [handler, this](ViewDestroyEvent*){
        view = nullptr;
        delete handler;
    };

    view->get_destroy_event_emitter()->add_listener(handler);
}

void RawHostWidget::draw_internal(DrawingContext* ctx, PUPPY::Widget* widget, bool shift) {
    PUPPY::Event::Render widget_event { false };
    widget->on_render(widget_event);

    HostRenderTarget* host_render_target = dynamic_cast<HostRenderTarget*>(widget->get_texture());

    if(host_render_target) {
        draw_texture(ctx, widget, shift, host_render_target);

    }


    RawHostWidget* host_widget = dynamic_cast<RawHostWidget*>(widget);

    if(host_widget) {
        for(auto &child: host_widget->get_children()) {
            child->on_render(widget_event);
            RawHostWidget::draw_internal(ctx, child, true);
        }
    }
}

void RawHostWidget::draw_texture(DrawingContext* ctx, PUPPY::Widget* widget, bool shift,
                                 const HostRenderTarget* host_render_target) {
    auto texture = host_render_target->get_texture();
    UIFillStyleTexture style { texture };

    Vec2f size = (Vec2f) texture->get_size();

    Vertex vertices[] = {
            {{0, 0}, {0, 1}},
            {{0, 1}, {0, 0}},
            {{1, 1}, {1, 0}},
            {{1, 0}, {1, 1}}
    };

    Vec2f widget_size = HostAppInterface::pvec2f_to_vec2f(widget->get_body().size);
    Vec2f widget_pos = HostAppInterface::pvec2f_to_vec2f(widget->get_body().position);

    for(auto & vertex : vertices) {
        vertex.position *= widget_size;
        vertex.shape_position *= size;
        if(shift) {
            vertex.position += widget_pos;
        }
    }

    ctx->set_fill_style(&style);
    ctx->fill_shape(vertices, 4, PrimitiveType::triangle_fan);
    ctx->set_fill_style(nullptr);
}

RawHostWidget::RawHostWidget(const PUPPY::WBody &body, PUPPY::Widget* parent) {

}

void* RawHostWidget::get_extra_data(void* arg) { return nullptr; }
