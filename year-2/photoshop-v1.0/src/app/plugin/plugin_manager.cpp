//
// Created by Артем on 29.11.2021.
//

#include <cstdlib>
#include <dlfcn.h>
#include "plugin_manager.hpp"
#include "plugin_std.hpp"
#include "../photoshop_view.hpp"
#include "../tools/plugin_tool.hpp"
#include "../tools/tool_manager.hpp"
#include "../tools/factory/plugin_tool_factory.hpp"

bool PluginManager::load_plugin(const char* name) {
    auto* lib = dlopen(name, RTLD_LAZY);

    if(!lib) {
        printf("Could not load %s: dlopen failed.", name);
        return false;
    }

    auto get_plugin_interface = (const PPluginInterface* (*)()) dlsym(lib, PGET_INTERFACE_FUNC);
    if(!get_plugin_interface) {
        printf("Could not load %s: get_plugin_interface symbol not found.", name);
        dlclose(lib);
        return false;
    }

    const PPluginInterface* interface = get_plugin_interface();
    auto status = interface->general.init(&app_interface);
    if(status != PPS_OK) {
        printf("Could not load %s: plugin failed to initialize.", name);
        dlclose(lib);
        return false;
    }

    auto info = interface->general.get_info();

    auto plugin = new Plugin { lib, interface, nullptr, info->type == PPT_EFFECT };

    if(info->type == PPT_TOOL) {
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

    // TODO: Be more like me and be less like you

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
    AppInterface::app_instance = app;
    app_interface.std_version = 0;
    app_interface.reserved = nullptr;

    app_interface.target.get_size = &AppInterface::Target::get_size;
    app_interface.target.get_pixels = &AppInterface::Target::get_pixels;

    app_interface.general.feature_level =      PFeatureLevel::PFL_SHADER_SUPPORT;
    app_interface.general.get_absolute_time = &AppInterface::General::get_absolute_time;
    app_interface.general.get_color         = &AppInterface::General::get_color;
    app_interface.general.get_size          = &AppInterface::General::get_size;
    app_interface.general.release_pixels    = &AppInterface::General::release_pixels;
    app_interface.general.log               = &AppInterface::General::log;

    app_interface.render.circle    = &AppInterface::Render::circle;
    app_interface.render.line      = &AppInterface::Render::line;
    app_interface.render.triangle  = &AppInterface::Render::triangle;
    app_interface.render.rectangle = &AppInterface::Render::rectangle;
    app_interface.render.pixels    = &AppInterface::Render::pixels;

    app_interface.extensions.enable   = &AppInterface::Extensions::enable;
    app_interface.extensions.get_func = &AppInterface::Extensions::get_func;

    app_interface.settings.get = &AppInterface::Settings::get;
    app_interface.settings.add = &AppInterface::Settings::add;
    app_interface.settings.create_surface = &AppInterface::Settings::create_surface;
    app_interface.settings.destroy_surface = &AppInterface::Settings::destroy_surface;

    app_interface.target.get_pixels = &AppInterface::Target::get_pixels;
    app_interface.target.get_size   = &AppInterface::Target::get_size;

    app_interface.shader.compile = &AppInterface::Shader::compile;
    app_interface.shader.apply = &AppInterface::Shader::apply;
    app_interface.shader.release = &AppInterface::Shader::release;
    app_interface.shader.set_uniform_float = &AppInterface::Shader::set_uniform_float;
    app_interface.shader.set_uniform_float_arr = &AppInterface::Shader::set_uniform_float_arr;
    app_interface.shader.set_uniform_int = &AppInterface::Shader::set_uniform_int;
    app_interface.shader.set_uniform_int_arr = &AppInterface::Shader::set_uniform_int_arr;

}

PluginManager::~PluginManager() {
    while(!plugins.empty()) {
        auto plugin = plugins[plugins.size() - 1];
        plugin->interface->general.deinit();
        dlclose(plugin->lib);
        plugins.pop_back();
    }
}

void PluginManager::set_active_plugin_tool(PluginTool* tool) {
    active_plugin_tool = tool;
}

PluginTool* PluginManager::get_active_plugin_tool() {
    return active_plugin_tool;
}
