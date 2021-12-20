//
// Created by Артем on 29.11.2021.
//

#include <cstdarg>
#include <cstdio>
#include <chrono>
#include "app_interface.hpp"
#include "../photoshop_view.hpp"
#include "../tools/tool_manager.hpp"
#include "host_render_target.hpp"
#include "host_widget_factory.hpp"
#include "host_render_target_factory.hpp"
#include "widgets/host_root_widget.hpp"
#include "../app.hpp"

const char* TWEAKS_EXT_NAME = "jm-tweaks";

bool HostAppInterface::ext_enable(const char* name) {
    if(strcmp(name, TWEAKS_EXT_NAME) == 0) {
        if(tweaks_extension) return true;
        tweaks_extension = new TweaksExtension(this);
        return true;
    }

    return false;
}

void* HostAppInterface::ext_get_func(const char* extension, const char* func) const {
    if(tweaks_extension && strcmp(extension, TWEAKS_EXT_NAME) == 0) {
        return tweaks_extension->get_func(func);
    }
    return nullptr;
}

void* HostAppInterface::ext_get_interface(const char* extension, const char* name) const {
    return nullptr;
}

void HostAppInterface::ext_register_as(const char* extension) const {

}

void HostAppInterface::log(const char* fmt, ...) const {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}

double HostAppInterface::get_absolute_time() const {
    auto now = std::chrono::system_clock::now();
    return (double) std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count() / (double) 1000000;
}

PUPPY::RGBA HostAppInterface::get_color() const {
    return vec4f_to_prgba(app->get_tool_manager()->get_color());
}

float HostAppInterface::get_size() const {
    return 10;
}

void HostAppInterface::set_color(const PUPPY::RGBA &color) const {
    app->get_tool_manager()->set_color(prgba_to_vec4f(color));
}

void HostAppInterface::set_size(float size) const {

}

const std::vector<PUPPY::WBody> &HostAppInterface::get_windows() const {
    return windows;
}

PUPPY::Widget* HostAppInterface::get_root_widget() {
    if(!root_widget) {
        auto root = app->get_plugin_manager()->root_widget;
        root_widget = new HostRootWidget(this, {{}, root->get_body().size}, root);
        root_widget->set_owner(plugin);
    }

    return root_widget;
}

PUPPY::RenderTarget* HostAppInterface::get_target() const {
    auto layer = get_active_layer();
    if(!layer) return nullptr;

    auto target = new HostRenderTarget(layer->get_texture(), plugin);
    target->link_to_layer(layer);
    return target;
}

PUPPY::RenderTarget* HostAppInterface::get_preview() const {
    auto layer = get_active_layer();
    if(!layer) return nullptr;

    auto target = new HostRenderTarget(layer->get_preview_texture(), plugin);
    target->link_to_layer(layer);
    return target;
}

void HostAppInterface::flush_preview() const {
    auto layer = get_active_layer();
    if(!layer) return;

    UIFillStyleTexture fill_style(layer->get_preview_texture());
    ctx->push_render_target(layer->get_texture());
    ctx->set_fill_style(&fill_style);
    ctx->fill_rect({}, (Vec2f) layer->get_size());
    ctx->pop_render_target();
    layer->get_preview_texture()->get_render_texture()->clear({0, 0, 0, 0});
}

PUPPY::RGBA HostAppInterface::vec4f_to_prgba(const Vec4f &color) {
    return {
        (unsigned char)(color[0] * 255),
        (unsigned char)(color[1] * 255),
        (unsigned char)(color[2] * 255),
        (unsigned char)(color[3] * 255)
    };
}

Vec4f HostAppInterface::prgba_to_vec4f(const PUPPY::RGBA &color) {
    return {
        float(color.r) / 255,
        float(color.g) / 255,
        float(color.b) / 255,
        float(color.a) / 255
    };
}

PUPPY::Vec2f HostAppInterface::vec2f_to_pvec2f(const Vec2f &position) {
    return {position[0], position[1]};
}

Vec2f HostAppInterface::pvec2f_to_vec2f(const PUPPY::Vec2f &position) {
    return {position.x, position.y};
}

PUPPY::Vec2s HostAppInterface::vec2i_to_pvec2s(const Vec2i &position) {
    return {(size_t)position[0], (size_t)position[1]};
}

Vec2i HostAppInterface::pvec2s_to_vec2i(const PUPPY::Vec2s &position) {
    return {(int)position.x, (int)position.y};
}

CanvasLayer* HostAppInterface::get_active_layer() const {
    auto manager = app->get_tool_manager();
    if(!manager) return nullptr;

    auto canvas = manager->get_active_canvas();
    if(!canvas) return nullptr;

    return canvas->get_active_layer();
}

HostAppInterface::HostAppInterface(PhotoshopView* app) : app(app) {
    feature_level = PUPPY::SHADER_SUPPORT;
    std_version = PUPPY::STD_VERSION;
    ctx = new DrawingContext();
    factory.shader = new HostAppShaderFactory(this);
    factory.widget = new HostWidgetFactory(this);
    factory.target = new HostRenderTargetFactory(this);
}

HostAppInterface::~HostAppInterface() {
    delete ctx;
    delete factory.shader;
    delete factory.widget;
    delete factory.target;
    delete tweaks_extension;
    delete root_widget;
}

void HostAppInterface::set_plugin(Plugin* p_plugin) {
    plugin = p_plugin;
    if(root_widget) {
        root_widget->set_owner(plugin);
    }
}
