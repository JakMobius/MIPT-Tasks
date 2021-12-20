#pragma once
#include "../lib_std/widgets/collection.h"
#include "host_widget.hpp"
#include "../../../ui/ui_button.hpp"

class HostButton : public RawHostWidget, public PUPPY::Button {
    PUPPY::Button::HandlerType handler {};

    UIButton* button = nullptr;

public:
    void set_handler(const HandlerType &handler_) override;

    HandlerType &get_handler() override;

    HostButton(const PUPPY::WBody& body, Widget* parent);

    void set_size(const PUPPY::Vec2f &size_) override;

    void set_caption(const char *text, size_t font_size, const PUPPY::Vec2f *pos) override;

    HostButton(const PUPPY::Vec2f &position, const char* text, Widget* parent);
};