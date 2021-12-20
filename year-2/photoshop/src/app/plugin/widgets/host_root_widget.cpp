//
// Created by Артем on 18.12.2021.
//

#include "host_root_widget.hpp"
#include "host_window_widget.hpp"
#include "../../photoshop_view.hpp"
#include "../app_interface.hpp"

HostRootWidget::HostRootWidget(HostAppInterface* interface, const PUPPY::WBody &body, PUPPY::Widget* parent): interface(interface), RawHostWidget(body, parent) {
    view = new ProxyView(this);
    view->set_position({body.position.x, body.position.y});
    view->set_size({body.size.x, body.size.y});
    set_view(view);
    if(parent) {
        parent->add_child(this);
    }
}

bool HostRootWidget::add_child(PUPPY::Widget* child) {
    auto window = dynamic_cast<HostWindow*>(child);
    if(window) {
        interface->get_app()->get_window_container()->add_window((UIWindow*) window->get_view());
        return true;
    } else {
        return RawHostWidget::add_child(child);
    }
}
