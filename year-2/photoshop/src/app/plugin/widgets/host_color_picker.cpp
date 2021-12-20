//
// Created by Артем on 15.12.2021.
//

#include "host_color_picker.hpp"

void HostColorPicker::set_handler(const PUPPY::ColorPicker::HandlerType &handler_) {
    handler = handler_;
}

PUPPY::ColorPicker::HandlerType &HostColorPicker::get_handler() {
    return handler;
}

PUPPY::RGBA HostColorPicker::get_color() {
    PUPPY::RGBA result;
    return result;
}

void HostColorPicker::set_color(PUPPY::RGBA color) {

}

HostColorPicker::HostColorPicker(const PUPPY::WBody &body, PUPPY::Widget* parent) : RawHostWidget(body, parent) {
    view = new ProxyView(this);
    view->set_position({body.position.x, body.position.y});
    view->set_size({body.size.x, body.size.y});
    set_view(view);
    if(parent) {
        parent->add_child(this);
    }
}
