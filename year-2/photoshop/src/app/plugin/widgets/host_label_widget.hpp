#pragma once

#include "../lib_std/widgets/collection.h"
#include "host_widget.hpp"

class HostLabel : public RawHostWidget, public PUPPY::Label {
public:
    HostLabel(const PUPPY::Vec2f &pos, const char* text, Widget* parent);

    std::string_view get_text() const override;

    void set_text(std::string_view text) override;

};