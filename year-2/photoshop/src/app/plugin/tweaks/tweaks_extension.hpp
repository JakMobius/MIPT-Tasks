#pragma once

class TweaksExtension;

#include "../app_interface.hpp"

class TweaksExtension {
    HostAppInterface* interface;

    std::function<void(bool)>* set_widgets_should_tick_func = nullptr;
    std::function<void(void)>* set_needs_redraw_func = nullptr;
public:
    explicit TweaksExtension(HostAppInterface* interface);

    ~TweaksExtension();

    void* get_func(const char* string);
};