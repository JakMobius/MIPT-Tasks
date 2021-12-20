//
// Created by Артем on 18.12.2021.
//

#include "tweaks_extension.hpp"

TweaksExtension::TweaksExtension(HostAppInterface* interface) : interface(interface) {
    set_widgets_should_tick_func = new std::function<void(bool)>();
    set_needs_redraw_func = new std::function<void(void)>();

    *set_widgets_should_tick_func = [=](bool should_tick) {
        interface->get_plugin()->set_widgets_should_tick(should_tick);
    };
    *set_needs_redraw_func = [=]() {
        interface->get_plugin()->set_widgets_needs_redraw();
    };
}

TweaksExtension::~TweaksExtension() {
    delete set_widgets_should_tick_func;
    delete set_needs_redraw_func;
}

void* TweaksExtension::get_func(const char* string) {
    if(strcmp(string, "set_widgets_should_tick") == 0) return set_widgets_should_tick_func;
    if(strcmp(string, "set_needs_redraw") == 0) return set_needs_redraw_func;
    return nullptr;
}
