#pragma once

#include "plugin_std.hpp"
#include "../photoshop_view.hpp"

class HostWidgetFactory : public PUPPY::WidgetFactory {

    HostAppInterface* interface;

public:
    HostWidgetFactory(HostAppInterface* interface): interface(interface) {}

    PUPPY::Button* button(const PUPPY::WBody &body, PUPPY::Widget* parent) const override;

    PUPPY::Button* button(const PUPPY::Vec2f &pos, const char* caption, PUPPY::Widget* parent) const override;

    PUPPY::Slider* slider(PUPPY::Slider::Type type, const PUPPY::WBody &body, PUPPY::Widget* parent) const override;

    PUPPY::TextField* text_field(const PUPPY::WBody &body, PUPPY::Widget* parent) const override;

    PUPPY::Window* window(const char* name, const PUPPY::WBody &body, PUPPY::Widget* parent) const override;

    PUPPY::ColorPicker* color_picker(const PUPPY::WBody &body, PUPPY::Widget* parent) const override;

    PUPPY::Label* label(const PUPPY::Vec2f &pos, const char* text, PUPPY::Widget* parent) const override;

    PUPPY::Widget* abstract(const PUPPY::WBody &body, PUPPY::Widget* parent) const override;

};