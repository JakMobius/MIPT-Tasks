#pragma once

class PhotoshopView;
class PluginTool;
struct Plugin;

#include <cstdio>
#include <dirent.h>
#include <cstring>
#include "plugin_std.hpp"
#include "app_interface.hpp"
#include "../tools/plugin_tool.hpp"

struct Plugin {
    void* lib;
    const PPluginInterface* interface;
    PluginTool* tool;
};

class PluginManager {
    PAppInterface app_interface {};
    PhotoshopView* app;
    std::vector<Plugin*> plugins;
    PluginTool* active_plugin_tool;

    void register_plugin(Plugin* plugin);
public:
    explicit PluginManager(PhotoshopView* app);
    ~PluginManager();

    bool load_plugin_dir(const char* path);
    bool load_plugin(const char* path);

    void set_active_plugin_tool(PluginTool* tool);

    PluginTool* get_active_plugin_tool();
};

#include "../tools/plugin_tool.hpp"