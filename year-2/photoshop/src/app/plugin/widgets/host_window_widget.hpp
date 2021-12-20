#pragma once

#include "../lib_std/widgets/collection.h"
#include "host_widget.hpp"
#include "../../../ui/window/ui_window.hpp"

class HostWindow : public RawHostWidget, public PUPPY::Window {
    PUPPY::Window::HandlerType show_handler {};
    PUPPY::Window::HandlerType hide_handler {};

    HostWidget* nested_container;
public:

    HostWindow(const PUPPY::WBody& body, PUPPY::Widget* parent, const char* title = nullptr);

    void on_render(const PUPPY::Event::Render &event) override;

    void set_show_handler(HandlerType &handler_) override;

    HandlerType &get_show_handler() override;

    void set_hide_handler(HandlerType &handler_) override;

    HandlerType &get_hide_handler() override;

    bool set_name(const char* name) override;

    const char* get_name() override;

    bool add_child(Widget *child) override;
};