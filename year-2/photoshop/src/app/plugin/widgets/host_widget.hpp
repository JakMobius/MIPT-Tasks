#pragma once

class HostWidget;

#include "../raw_host_widget.hpp"

class HostWidget : public RawHostWidget {
public:
    HostWidget(const PUPPY::WBody& body, PUPPY::Widget* parent);
};
