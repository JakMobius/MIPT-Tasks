//
// Created by Артем on 29.11.2021.
//

#include <cstdlib>
#include <dlfcn.h>
#include "plugin_manager.hpp"
#include "../photoshop_view.hpp"
#include "../tools/plugin_tool.hpp"
#include "../tools/tool_manager.hpp"
#include "../tools/factory/plugin_tool_factory.hpp"
#include "widgets/host_root_widget.hpp"
#include "app_interface.hpp"
#include "widgets/host_widget.hpp"

bool PluginManager::load_plugin(const char* name) {
    auto* lib = dlopen(name, RTLD_LAZY);

    if(!lib) {
        printf("Could not load %s: dlopen failed.", name);
        return false;
    }

    auto get_plugin_interface = (const PUPPY::PluginInterface* (*)()) dlsym(lib, PUPPY::GET_INTERFACE_FUNC);
    if(!get_plugin_interface) {
        printf("Could not load %s: get_plugin_interface symbol not found.", name);
        dlclose(lib);
        return false;
    }

    const PUPPY::PluginInterface* interface = get_plugin_interface();
    auto host_interface = new HostAppInterface(this->app);

    auto info = interface->get_info();

    if(info->std_version != PUPPY::STD_VERSION) {
        printf("Could not load %s: plugin std version is 0x%x, host std version is 0x%x\n", name, info->std_version, PUPPY::STD_VERSION);
        dlclose(lib);
        delete host_interface;
        return false;
    }

    auto plugin = new Plugin { lib, interface, host_interface, nullptr, info->type == PUPPY::EFFECT };
    host_interface->set_plugin(plugin);

    auto status = interface->init(host_interface);
    if(status != PUPPY::OK) {
        printf("Could not load %s: plugin failed to initialize.", name);
        dlclose(lib);
        delete host_interface;
        return false;
    }

    if(info->type == PUPPY::TOOL) {
        app->get_tool_manager()->add_tool_factory(new PluginToolFactory(plugin));
    }

    register_plugin(plugin);
    return true;
}

bool PluginManager::load_plugin_dir(const char* path) {
    DIR *directory = opendir (path);

    if (!directory) return false;

#if __APPLE__
    const char* extension = ".dylib";
#else
    const char* extension = ".so";
#endif

    size_t extension_length = strlen(extension);
    size_t path_length = strlen(path);
    if(path[path_length - 1] != '/') path_length++;

    struct dirent *ep = nullptr;
    while ((ep = readdir(directory))) {

        size_t dir_name_length = strlen(ep->d_name);

        if(dir_name_length < extension_length) continue;
        if(strcmp(extension, ep->d_name + dir_name_length - extension_length) != 0) continue;

        char* plugin_path = (char*) calloc(1, path_length + dir_name_length + 1);

        sprintf(plugin_path, "%s", path);
        sprintf(plugin_path + path_length - 1, "/%s", ep->d_name);
        load_plugin(plugin_path);

        free(plugin_path);
    }

    (void) closedir(directory);
    return true;
}

void PluginManager::register_plugin(Plugin* plugin) {
    plugins.push_back(plugin);
}

PluginManager::PluginManager(PhotoshopView* app) : app(app) {

    root_widget = new HostWidget({{}, HostAppInterface::vec2f_to_pvec2f(app->get_size())}, nullptr);
    app->get_window_container()->append_child(root_widget->get_view());
}

PluginManager::~PluginManager() {
    while(!plugins.empty()) {
        auto plugin = plugins[plugins.size() - 1];
        plugin->interface->deinit();
        dlclose(plugin->lib);
        plugins.pop_back();
    }
}

void Plugin::set_widgets_should_tick(bool should_tick) {
    widgets_should_tick = should_tick;
    if(should_tick) set_widgets_needs_redraw();
}

void Plugin::set_widgets_needs_redraw() {
    this->host_interface->get_app()->set_needs_redraw();
}
