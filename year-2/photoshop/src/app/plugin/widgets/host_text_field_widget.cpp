//
// Created by Артем on 15.12.2021.
//

#include "host_text_field_widget.hpp"

void HostTextField::set_handler(const PUPPY::TextField::HandlerType &handler_) {
    handler = handler_;
}

PUPPY::TextField::HandlerType &HostTextField::get_handler() {
    return handler;
}

std::string_view HostTextField::get_text() const {
    return std::string_view();
}

void HostTextField::set_text(std::string_view text) {

}

HostTextField::HostTextField(const PUPPY::WBody &body, PUPPY::Widget* parent) : RawHostWidget(body, parent) {
    view = new ProxyView(this);
    view->set_position({body.position.x, body.position.y});
    view->set_size({body.size.x, body.size.y});
    set_view(view);
    if(parent) {
        parent->add_child(this);
    }
}
