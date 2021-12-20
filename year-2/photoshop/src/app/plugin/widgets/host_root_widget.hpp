#pragma once

#include "../raw_host_widget.hpp"
#include "../plugin_manager.hpp"

class HostRootWidget : public RawHostWidget {
    HostAppInterface* interface;
public:
    HostRootWidget(HostAppInterface* interface, const PUPPY::WBody& body, PUPPY::Widget* parent);

    bool add_child(Widget *child) override;
};