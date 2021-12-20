#pragma once

class PhotoshopView;
class CanvasLayer;
class HostAppInterface;

#include "plugin_std.hpp"
#include "../../graphics/drawing_context.hpp"
#include "../../ui/styles/fill_style/fill_style_color.hpp"
#include "../../ui/styles/fill_style/fill_style_texture.hpp"
#include "plugin_manager.hpp"
#include "host_shader_factory.hpp"
#include "tweaks/tweaks_extension.hpp"
#include "widgets/host_root_widget.hpp"

class HostAppInterface : public PUPPY::AppInterface {

    std::vector<PUPPY::WBody> windows = {};
    PhotoshopView* app;
    Plugin* plugin;
    DrawingContext* ctx;
    TweaksExtension* tweaks_extension = nullptr;
    mutable HostRootWidget* root_widget = nullptr;

    CanvasLayer* get_active_layer() const;

public:
    HostAppInterface(PhotoshopView* app);

    DrawingContext* get_drawing_context() { return ctx; }

    virtual ~HostAppInterface();

    void set_plugin(Plugin* p_plugin);
    Plugin* get_plugin() const { return plugin; }

    bool ext_enable(const char *name) override;
    void *ext_get_func(const char *extension, const char *func) const override;
    void *ext_get_interface(const char *extension, const char *name) const override;
    void ext_register_as(const char *extension) const override;

    void log(const char *fmt, ...) const override;
    double get_absolute_time()     const override;

    PUPPY::RGBA get_color() const override;
    float get_size() const override;

    void set_color(const PUPPY::RGBA &color) const override;
    void set_size(float size) const override;

    const std::vector<PUPPY::WBody> &get_windows() const override;
    PUPPY::Widget *get_root_widget() override;

    PUPPY::RenderTarget *get_target()  const override; // returns actual active  layer, drawing in it changes app's layer
    PUPPY::RenderTarget *get_preview() const override; // returns actual preview layer, drawing in it changes app's layer
    void flush_preview()        const override;

    static PUPPY::RGBA vec4f_to_prgba(const Vec4f &color);
    static Vec4f prgba_to_vec4f(const PUPPY::RGBA &color);
    static PUPPY::Vec2f vec2f_to_pvec2f(const Vec2f &position);
    static Vec2f pvec2f_to_vec2f(const PUPPY::Vec2f &position);
    static PUPPY::Vec2s vec2i_to_pvec2s(const Vec2i &position);
    static Vec2i pvec2s_to_vec2i(const PUPPY::Vec2s &position);

    PhotoshopView* get_app() const { return app; }

};