//
// Created by Артем on 15.12.2021.
//

#include "host_slider_widget.hpp"

void HostSlider::set_handler(const PUPPY::Slider::HandlerType &handler_) {
    handler = handler_;
}

PUPPY::Slider::HandlerType &HostSlider::get_handler() {
    return handler;
}

PUPPY::Vec2f HostSlider::get_fraction_2d() {
    return {0, 0};
}

void HostSlider::set_fraction_2d(const PUPPY::Vec2f &frac) {

}

float HostSlider::get_fraction() {
    return 0;
}

void HostSlider::set_fraction(float frac) {

}

HostSlider::HostSlider(PUPPY::Slider::Type type, const PUPPY::WBody& body, PUPPY::Widget* parent): RawHostWidget(body, parent) {
    view = new ProxyView(this);
    view->set_position({body.position.x, body.position.y});
    view->set_size({body.size.x, body.size.y});
    set_view(view);
    if(parent) {
        parent->add_child(this);
    }
}
