#pragma once

class PhotoshopView;
class PluginTool;

#include <cstdio>
#include <dirent.h>
#include <cstring>
#include "../tools/plugin_tool.hpp"
#include "raw_host_widget.hpp"
#include "plugin.hpp"

class PluginManager {
    PhotoshopView* app;
    std::vector<Plugin*> plugins {};

    void register_plugin(Plugin* plugin);
public:
    explicit PluginManager(PhotoshopView* app);
    ~PluginManager();

    bool load_plugin_dir(const char* path);
    bool load_plugin(const char* path);

    PhotoshopView* get_app() { return app; }

    const std::vector<Plugin*>& get_plugins() { return plugins; }

    RawHostWidget* root_widget;
};

#include "../tools/plugin_tool.hpp"