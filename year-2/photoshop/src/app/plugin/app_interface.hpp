#pragma once

class PhotoshopView;
class CanvasLayer;

#include "plugin_std.hpp"
#include "../../graphics/drawing_context.hpp"
#include "../../ui/styles/fill_style/fill_style_color.hpp"
#include "../../ui/styles/fill_style/fill_style_texture.hpp"

namespace AppInterface {
    extern PhotoshopView* app_instance;

    namespace Utils {
        extern DrawingContext shared_context;
        extern UIFillStyleTexture shared_fill_style;

        PRGBA vec4f_to_prgba(const Vec4f& color);
        Vec4f prgba_to_vec4f(const PRGBA& color);
        PVec2f vec2f_to_pvec2f(const Vec2f& position);
        Vec2f pvec2f_to_vec2f(const PVec2f& position);
        CanvasLayer* get_current_layer();


        void fuse_context(const PRGBA& color, const PRenderMode* render_mode);
        void unfuse_context();
        void fill_shape(Vec2f* shape, int count, PrimitiveType type);
    }

    namespace Extensions {
        bool enable(const char* name);
        void* get_func(const char* name);
    }

    namespace General {
        void log(const char *fmt, ...);
        double get_absolute_time();

        void release_pixels(PRGBA* pixels);

        PRGBA get_color();
        float get_size();
    }

    namespace Target {
        PRGBA* get_pixels();
        void get_size(size_t* width, size_t* height);
    }

    namespace Render {
        void circle(PVec2f position, float radius, PRGBA color, const PRenderMode *render_mode);
        void line(PVec2f start, PVec2f end, PRGBA color, const PRenderMode *render_mode);
        void triangle(PVec2f p1, PVec2f p2, PVec2f p3, PRGBA color, const PRenderMode *render_mode);
        void rectangle(PVec2f p1, PVec2f p2, PRGBA color, const PRenderMode *render_mode);

        void pixels(PVec2f position, const PRGBA *data, size_t width, size_t height, const PRenderMode *render_mode);
    }

    namespace Settings {
        void  create_surface (size_t width, size_t height);
        void  destroy_surface();

        void* add(PSettingType type, const char *name);
        void  get(void *handle, void *answer);
    }

    namespace Shader {
        void apply(void *shader, const PRenderMode *render_mode);

        void *compile(const char *code, PShaderType type);
        void  release(void *);

        void set_uniform_int    (const char *name, int  val);
        void set_uniform_int_arr(const char *name, int *val, size_t cnt);

        void set_uniform_float    (const char *name, float  val);
        void set_uniform_float_arr(const char *name, float *val, size_t cnt);
    }
}