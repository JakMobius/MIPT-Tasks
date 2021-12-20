#pragma once

#include "host_widget.hpp"
#include "../lib_std/widgets/collection.h"

class HostColorPicker : public RawHostWidget, public PUPPY::ColorPicker {
    PUPPY::ColorPicker::HandlerType handler;

public:
    void set_handler(const HandlerType &handler_) override;

    HandlerType &get_handler() override;

    PUPPY::RGBA get_color() override;

    void set_color(PUPPY::RGBA color) override;

    HostColorPicker(const PUPPY::WBody &body, Widget* parent);
};