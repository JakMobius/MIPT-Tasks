//
// Created by Артем on 15.12.2021.
//

#include "host_label_widget.hpp"

std::string_view HostLabel::get_text() const {
    return {};
}

void HostLabel::set_text(std::string_view text) {

}

HostLabel::HostLabel(const PUPPY::Vec2f &pos, const char* text, PUPPY::Widget* parent): RawHostWidget({}, parent) {
    view = new ProxyView(this);
    view->set_position({body.position.x, body.position.y});
    view->set_size({body.size.x, body.size.y});
    set_view(view);
    if(parent) {
        parent->add_child(this);
    }
}
