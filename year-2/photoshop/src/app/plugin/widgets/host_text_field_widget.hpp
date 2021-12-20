#pragma once

#include "host_widget.hpp"
#include "../lib_std/widgets/collection.h"

class HostTextField : public RawHostWidget, public PUPPY::TextField {
    PUPPY::TextField::HandlerType handler {};

public:
    void set_handler(const HandlerType &handler_) override;

    HandlerType &get_handler() override;

    std::string_view get_text() const override;

    void set_text(std::string_view text) override;

    HostTextField(const PUPPY::WBody &body, Widget* parent);
};