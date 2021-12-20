#pragma once

struct Plugin;
class HostAppInterface;

#include "plugin_std.hpp"
#include "../tools/plugin_tool.hpp"

struct Plugin {
    void* lib = nullptr;
    const PUPPY::PluginInterface* interface = nullptr;
    HostAppInterface* host_interface = nullptr;
    PluginTool* tool = nullptr;
    bool is_effect = false;
    bool widgets_should_tick = true;

    void set_widgets_should_tick(bool should_tick);

    void set_widgets_needs_redraw();
};