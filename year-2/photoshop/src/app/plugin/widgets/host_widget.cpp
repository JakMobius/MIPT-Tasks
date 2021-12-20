//
// Created by Артем on 14.12.2021.
//

#include "host_widget.hpp"

HostWidget::HostWidget(const PUPPY::WBody &body, PUPPY::Widget* parent) : RawHostWidget(body, parent) {
    view = new ProxyView(this);
    view->set_position({body.position.x, body.position.y});
    view->set_size({body.size.x, body.size.y});
    set_view(view);
    if(parent) {
        parent->add_child(this);
    }
}
