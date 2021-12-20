#pragma once

class PluginTool;

#include "tool.hpp"
#include "../plugin/plugin_manager.hpp"
#include "../../ui/styles/fill_style/fill_style_texture.hpp"
#include "../plugin/plugin.hpp"

class PluginTool : public Tool {

    UIFillStyleTexture brush_apply_style {};
    Vec2f old_position {};
    Plugin* plugin;

public:
    PluginTool(Plugin* plugin): Tool(), plugin(plugin) {}
    virtual ~PluginTool() override;

    void on_mouse_down(Vec2f position) override;
    void on_mouse_move(Vec2f position) override;
    void on_mouse_up() override;

    void on_become_active() override;
    void on_resign_active() override;

    void setup_color_blending();
};