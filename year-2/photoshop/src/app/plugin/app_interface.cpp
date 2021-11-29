//
// Created by Артем on 29.11.2021.
//

#include <cstdarg>
#include <cstdio>
#include <chrono>
#include "app_interface.hpp"
#include "../photoshop_view.hpp"
#include "../tools/tool_manager.hpp"
#include "../../graphics/shapes/circle_shape.hpp"

PhotoshopView* AppInterface::app_instance = nullptr;
DrawingContext AppInterface::Utils::shared_context {};
UIFillStyleTexture AppInterface::Utils::shared_fill_style {};

void AppInterface::General::log(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}

double AppInterface::General::get_absolute_time() {
    auto now = std::chrono::system_clock::now();

    return (double) std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count() / (double) 1000000;
}

void AppInterface::General::release_pixels(PRGBA* pixels) {
    delete[] pixels;
}

PRGBA AppInterface::General::get_color() {
    if(app_instance->get_tool_manager() == nullptr) return {};
    return Utils::vec4f_to_prgba(app_instance->get_tool_manager()->get_color());
}

float AppInterface::General::get_size() {
    return 10;
}

PRGBA* AppInterface::Target::get_pixels() {
    auto layer = Utils::get_current_layer();
    if(!layer) return nullptr;

    sf::Image img = layer->get_texture()->get_texture()->copyToImage();
    auto img_pixels = img.getPixelsPtr();
    auto size = img.getSize();

    PRGBA *pixels = new PRGBA[size.x * size.y];

    for(int y = size.y - 1, y1 = 0; y >= 0; y--, y1++) {
        int i1 = y * size.x;
        int i2 = y1 * size.y;

        for(int x = 0; x < size.x; x++) {
            pixels[i1 + x] = ((PRGBA*)img_pixels)[i2 + x];
        }
    }

    return pixels;
}

void AppInterface::Target::get_size(size_t* width, size_t* height) {
    auto layer = Utils::get_current_layer();
    if(!layer) {
        *width = 0;
        *height = 0;
    } else {
        auto size = layer->get_size();
        *width = size[0];
        *height = size[1];
    }
}

PRGBA AppInterface::Utils::vec4f_to_prgba(const Vec4f &color) {
    return {
        (unsigned char)(color[0] * 255),
        (unsigned char)(color[1] * 255),
        (unsigned char)(color[2] * 255),
        (unsigned char)(color[3] * 255)
    };
}

Vec4f AppInterface::Utils::prgba_to_vec4f(const PRGBA &color) {
    return {
        float(color.r) / 255,
        float(color.g) / 255,
        float(color.b) / 255,
        float(color.a) / 255
    };
}

CanvasLayer* AppInterface::Utils::get_current_layer() {
    auto tool_manager = app_instance->get_tool_manager();
    if (!tool_manager) return nullptr;

    auto canvas = tool_manager->get_active_canvas();
    if (!canvas) return nullptr;

    auto layer = canvas->get_active_layer();
    return layer;
}

PVec2f AppInterface::Utils::vec2f_to_pvec2f(const Vec2f &position) {
    return {position[0], position[1]};
}

Vec2f AppInterface::Utils::pvec2f_to_vec2f(const PVec2f &position) {
    return {position.x, position.y};
}

void AppInterface::Utils::fill_shape(Vec2f* shape, int count, PrimitiveType type) {
    auto layer = Utils::get_current_layer();
    if(!layer) return;

    if(count < 128) {
        Vertex vertices[count];

        for(int i = 0; i < count; i++) {
            vertices[i].position = shape[i];
            vertices[i].shape_position = shape[i];
        }

        Utils::shared_context.fill_shape(vertices, count, type);
    } else {
        auto vertices = (Vertex*)calloc(count, sizeof(Vertex));

        for(int i = 0; i < count; i++) {
            vertices[i].position = shape[i];
            vertices[i].shape_position = shape[i];
        }

        Utils::shared_context.fill_shape(vertices, count, type);
        free(vertices);
    }
}

void AppInterface::Utils::fuse_context(const PRGBA &color, const PRenderMode* mode) {
    auto layer = Utils::get_current_layer();
    if(!layer) return;

    auto* context = &Utils::shared_context;

    if(mode->draw_policy == PPDP_ACTIVE) {
        context->push_render_target(layer->get_texture());
    } else {
        context->push_render_target(layer->get_preview_texture());
    }

    if(mode->blend == PPBM_ALPHA_BLEND) {
        Utils::shared_fill_style.get_render_states()->blendMode = sf::BlendAlpha;
    } else {
        Utils::shared_fill_style.get_render_states()->blendMode = sf::BlendNone;
    }

    shared_fill_style.set_color(prgba_to_vec4f(color));
    context->set_fill_style(&Utils::shared_fill_style);
}

void AppInterface::Utils::unfuse_context() {
    auto layer = Utils::get_current_layer();
    if(!layer) return;

    auto* context = &Utils::shared_context;
    context->pop_render_target();

    layer->set_needs_redraw();
}

void AppInterface::Render::circle(PVec2f position, float radius, PRGBA color, const PRenderMode* render_mode) {

    Utils::fuse_context(color, render_mode);
    CircleShape shape(Utils::pvec2f_to_vec2f(position), radius);
    shape.draw(&Utils::shared_context);
    Utils::unfuse_context();
}

void AppInterface::Render::line(PVec2f start, PVec2f end, PRGBA color, const PRenderMode* render_mode) {
    auto layer = Utils::get_current_layer();
    if(!layer) return;

    Vec2f shape[] = {
        Utils::pvec2f_to_vec2f(start),
        Utils::pvec2f_to_vec2f(end)
    };

    Utils::fuse_context(color, render_mode);
    Utils::fill_shape(shape, 2, PrimitiveType::lines);
    Utils::unfuse_context();
}

void AppInterface::Render::triangle(PVec2f p1, PVec2f p2, PVec2f p3, PRGBA color, const PRenderMode* render_mode) {
    auto layer = Utils::get_current_layer();
    if(!layer) return;

    Vec2f shape[] = {
        Utils::pvec2f_to_vec2f(p1),
        Utils::pvec2f_to_vec2f(p2),
        Utils::pvec2f_to_vec2f(p3)
    };

    Utils::fuse_context(color, render_mode);
    Utils::fill_shape(shape, 3, PrimitiveType::triangles);
    Utils::unfuse_context();
}

void AppInterface::Render::rectangle(PVec2f p1, PVec2f p2, PRGBA color, const PRenderMode* render_mode) {
    auto layer = Utils::get_current_layer();
    if(!layer) return;

    Vec2f shape[] = {
        {p1.x, p1.y},
        {p2.x, p1.y},
        {p2.x, p2.y},
        {p1.x, p2.y}
    };

    Utils::fuse_context(color, render_mode);
    Utils::fill_shape(shape, 4, PrimitiveType::triangle_fan);
    Utils::unfuse_context();
}

void AppInterface::Render::pixels(PVec2f position, const PRGBA* data, size_t width, size_t height,
                                  const PRenderMode* render_mode) {
    sf::Texture texture;
    texture.create(width, height);
    texture.update((sf::Uint8*) data);
    texture.setRepeated(true);

    Utils::fuse_context({255, 255, 255, 255}, render_mode);
    Utils::shared_fill_style.get_render_states()->texture = &texture;
    Utils::shared_context.transform = Utils::shared_context.transform.multiplied(Matrix3f::scale_matrix(1, -1));
    Utils::shared_context.fill_rect(Utils::pvec2f_to_vec2f(position) - Vec2f { 0, (float)height }, { (float)width, (float)height});
    Utils::shared_context.transform = Matrix3f();
    Utils::shared_fill_style.get_render_states()->texture = nullptr;
    Utils::unfuse_context();
}

void AppInterface::Settings::create_surface(size_t width, size_t height) {

}

void AppInterface::Settings::destroy_surface() {

}

void* AppInterface::Settings::add(PSettingType type, const char* name) {
    return nullptr;
}

void AppInterface::Settings::get(void* handle, void* answer) {

}

void AppInterface::Shader::apply(void* shader, const PRenderMode* render_mode) {

}

void* AppInterface::Shader::compile(const char* code, PShaderType type) {
    return nullptr;
}

void AppInterface::Shader::release(void*) {

}

void AppInterface::Shader::set_uniform_int(const char* name, int val) {

}

void AppInterface::Shader::set_uniform_int_arr(const char* name, int* val, size_t cnt) {

}

void AppInterface::Shader::set_uniform_float(const char* name, float val) {

}

void AppInterface::Shader::set_uniform_float_arr(const char* name, float* val, size_t cnt) {

}

bool AppInterface::Extensions::enable(const char* name) { return false; }

void* AppInterface::Extensions::get_func(const char* name) { return nullptr; }
