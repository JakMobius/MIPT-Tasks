#pragma once

#include "tool_factory.hpp"
#include "../../plugin/plugin_manager.hpp"

class PluginToolFactory : public ToolFactoryBase {

    Plugin* plugin;

public:
    explicit PluginToolFactory(Plugin* plugin): ToolFactoryBase(), plugin(plugin) {
        tool_name = plugin->interface->get_info()->name;
        tool_icon = nullptr;
    }

    Tool* create_tool() override {
        if(plugin->tool) return plugin->tool;
        auto tool = new PluginTool(plugin);
        plugin->tool = tool;
        return tool;
    }
};