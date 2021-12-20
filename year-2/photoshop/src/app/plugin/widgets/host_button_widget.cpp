//
// Created by Артем on 15.12.2021.
//

#include "host_button_widget.hpp"

void HostButton::set_handler(const PUPPY::Button::HandlerType &handler_) {
    handler = handler_;
}

PUPPY::Button::HandlerType &HostButton::get_handler() {
    return handler;
}

HostButton::HostButton(const PUPPY::WBody& body, PUPPY::Widget* parent): RawHostWidget(body, parent) {
    view = new ProxyView(this);
    view->set_position({body.position.x, body.position.y});
    view->set_size({body.size.x, body.size.y});

    button = new UIButton();
    button->set_size(view->get_size());
    button->set_callback([=]() {
        if(handler) handler();
    });
    view->append_child(button);

    set_view(view);
    if(parent) {
        parent->add_child(this);
    }
}

HostButton::HostButton(const PUPPY::Vec2f &position, const char* text, PUPPY::Widget* parent): RawHostWidget({}, parent) {
    view = new ProxyView(this);
    view->set_position({position.x, position.y});
    view->set_size({50, 50});

    button = new UIButton();
    button->set_size(view->get_size());
    button->set_title(text);
    button->set_callback([=]() {
       if(handler) handler();
    });
    view->append_child(button);

    set_view(view);
    if(parent) {
        parent->add_child(this);
    }
}

void HostButton::set_size(const PUPPY::Vec2f &size_) {
    RawHostWidget::set_size(size_);
    get_body();
    button->set_size({body.size.x, body.size.y});
}

void HostButton::set_caption(const char* text, size_t font_size, const PUPPY::Vec2f* pos) {
    RawHostWidget::set_caption(text, font_size, pos);
    button->set_title(text);
}
