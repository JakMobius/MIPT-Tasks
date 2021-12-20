#pragma once

#include "../lib_std/widgets/collection.h"
#include "host_widget.hpp"

class HostSlider : public virtual RawHostWidget, public PUPPY::Slider {
    PUPPY::Slider::HandlerType handler {};
public:
    void set_handler(const HandlerType &handler_) override;

    HandlerType &get_handler() override;

    PUPPY::Vec2f get_fraction_2d() override;

    void set_fraction_2d(const PUPPY::Vec2f &frac) override;

    float get_fraction() override;

    void set_fraction(float frac) override;

    HostSlider(PUPPY::Slider::Type type, const PUPPY::WBody& body, PUPPY::Widget* parent);
};