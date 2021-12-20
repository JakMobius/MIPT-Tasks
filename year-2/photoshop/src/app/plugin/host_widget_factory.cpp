//
// Created by Артем on 14.12.2021.
//

#include "host_widget_factory.hpp"
#include "widgets/host_widget.hpp"
#include "widgets/host_button_widget.hpp"
#include "widgets/host_slider_widget.hpp"
#include "widgets/host_window_widget.hpp"
#include "widgets/host_label_widget.hpp"
#include "widgets/host_text_field_widget.hpp"
#include "widgets/host_color_picker.hpp"
#include "app_interface.hpp"

PUPPY::Button* HostWidgetFactory::button(const PUPPY::WBody &body, PUPPY::Widget* parent) const {
    auto widget = new HostButton(body, parent);
    widget->set_owner(interface->get_plugin());
    return widget;
}

PUPPY::Button* HostWidgetFactory::button(const PUPPY::Vec2f &pos, const char* caption, PUPPY::Widget* parent) const {
    auto widget = new HostButton(pos, caption, parent);
    widget->set_owner(interface->get_plugin());
    return widget;
}

PUPPY::Slider*
HostWidgetFactory::slider(PUPPY::Slider::Type type, const PUPPY::WBody &body, PUPPY::Widget* parent) const {
    auto widget = new HostSlider(type, body, parent);
    widget->set_owner(interface->get_plugin());
    return widget;
}

PUPPY::TextField* HostWidgetFactory::text_field(const PUPPY::WBody &body, PUPPY::Widget* parent) const {
    auto widget = new HostTextField(body, parent);
    widget->set_owner(interface->get_plugin());
    return widget;
}

PUPPY::Window* HostWidgetFactory::window(const char* name, const PUPPY::WBody &body, PUPPY::Widget* parent) const {
    auto widget = new HostWindow(body, parent, name);
    widget->set_owner(interface->get_plugin());
    return widget;
}

PUPPY::ColorPicker* HostWidgetFactory::color_picker(const PUPPY::WBody &body, PUPPY::Widget* parent) const {
    auto widget = new HostColorPicker(body, parent);
    widget->set_owner(interface->get_plugin());
    return widget;
}

PUPPY::Label* HostWidgetFactory::label(const PUPPY::Vec2f &pos, const char* text, PUPPY::Widget* parent) const {
    auto widget = new HostLabel(pos, text, parent);
    widget->set_owner(interface->get_plugin());
    return widget;
}

PUPPY::Widget* HostWidgetFactory::abstract(const PUPPY::WBody &body, PUPPY::Widget* parent) const {
    auto widget = new HostWidget(body, parent);
    widget->set_owner(interface->get_plugin());
    return widget;
}
