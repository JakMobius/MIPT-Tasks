//
// Created by Артем on 14.12.2021.
//

#include "host_render_target.hpp"
#include "app_interface.hpp"
#include "../../graphics/shapes/circle_shape.hpp"
#include "../canvas/canvas_layer.hpp"

PUPPY::RenderTarget* HostRenderTarget::get_copy() const {
    return new HostRenderTarget(new DrawingTargetTexture(*target), plugin);
}

PUPPY::Vec2s HostRenderTarget::get_size() const {
    return HostAppInterface::vec2i_to_pvec2s(target->get_size());
}

PUPPY::RGBA HostRenderTarget::get_pixel(size_t x, size_t y) const {
    PUPPY::RGBA result;
    return result;
}

void HostRenderTarget::set_pixel(size_t x, size_t y, const PUPPY::RGBA &color) {

}

PUPPY::RGBA* HostRenderTarget::get_pixels() const {

    sf::Image img = target->get_texture()->copyToImage();
    auto img_pixels = img.getPixelsPtr();
    auto size = img.getSize();

    PUPPY::RGBA *pixels = new PUPPY::RGBA[size.x * size.y];

    for(int y = size.y - 1, y1 = 0; y >= 0; y--, y1++) {
        int i1 = y * size.x;
        int i2 = y1 * size.x;

        for(int x = 0; x < size.x; x++) {
            pixels[i1 + x] = ((PUPPY::RGBA*)img_pixels)[i2 + x];
        }
    }

    return pixels;
}

void HostRenderTarget::clear(const PUPPY::RGBA &color) {

}

void HostRenderTarget::render_circle(const PUPPY::Vec2f &position, float radius, const PUPPY::RGBA &color,
                                     const PUPPY::RenderMode &render_mode) {
    fuse_context(color, render_mode);
    CircleShape shape(HostAppInterface::pvec2f_to_vec2f(position), radius);
    shape.draw(plugin->host_interface->get_drawing_context());
    unfuse_context();
}

void HostRenderTarget::render_line(const PUPPY::Vec2f &start, const PUPPY::Vec2f &end, const PUPPY::RGBA &color,
                                   const PUPPY::RenderMode &render_mode) {
    Vec2f shape[] = {
        HostAppInterface::pvec2f_to_vec2f(start),
        HostAppInterface::pvec2f_to_vec2f(end)
    };

    fuse_context(color, render_mode);
    fill_shape(shape, 2, PrimitiveType::lines);
    unfuse_context();
}

void HostRenderTarget::render_triangle(const PUPPY::Vec2f &p1, const PUPPY::Vec2f &p2, const PUPPY::Vec2f &p3,
                                       const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode) {
    Vec2f shape[] = {
        HostAppInterface::pvec2f_to_vec2f(p1),
        HostAppInterface::pvec2f_to_vec2f(p2),
        HostAppInterface::pvec2f_to_vec2f(p3)
    };

    fuse_context(color, render_mode);
    fill_shape(shape, 3, PrimitiveType::triangles);
    unfuse_context();
}

void HostRenderTarget::render_rectangle(const PUPPY::Vec2f &p1, const PUPPY::Vec2f &p2, const PUPPY::RGBA &color,
                                        const PUPPY::RenderMode &render_mode) {
    Vec2f shape[] = {
        {p1.x, p1.y},
        {p2.x, p1.y},
        {p2.x, p2.y},
        {p1.x, p2.y}
    };

   fuse_context(color, render_mode);
   fill_shape(shape, 4, PrimitiveType::triangle_fan);
   unfuse_context();

}

void HostRenderTarget::fill_shape(Vec2f* shape, int count, PrimitiveType type) {

    if(count < 128) {
        Vertex vertices[count];

        for(int i = 0; i < count; i++) {
            vertices[i].position = shape[i];
            vertices[i].shape_position = shape[i];
        }

        plugin->host_interface->get_drawing_context()->fill_shape(vertices, count, type);
    } else {
        auto vertices = (Vertex*)calloc(count, sizeof(Vertex));

        for(int i = 0; i < count; i++) {
            vertices[i].position = shape[i];
            vertices[i].shape_position = shape[i];
        }

        plugin->host_interface->get_drawing_context()->fill_shape(vertices, count, type);
        free(vertices);
    }
}

void HostRenderTarget::render_texture(const PUPPY::Vec2f &position, const PUPPY::RenderTarget* texture,
                                      const PUPPY::RenderMode &render_mode) {

}

void HostRenderTarget::render_pixels(const PUPPY::Vec2f &position, const PUPPY::Vec2s &size, const PUPPY::RGBA* data,
                                     const PUPPY::RenderMode &render_mode) {
    sf::Texture texture;
    texture.create(size.x, size.y);
    texture.update((sf::Uint8*) data);
    texture.setRepeated(true);

    auto ctx = plugin->host_interface->get_drawing_context();

    fuse_context({255, 255, 255, 255}, render_mode);
    shared_fill_style.get_render_states()->texture = &texture;
    ctx->transform = ctx->transform.multiplied(Matrix3f::scale_matrix(1, -1));
    ctx->fill_rect(HostAppInterface::pvec2f_to_vec2f(position) - Vec2f { 0, (float)size.y }, { (float)size.x, (float)size.y});
    ctx->transform = Matrix3f();
    shared_fill_style.get_render_states()->texture = nullptr;
    unfuse_context();
}

void HostRenderTarget::apply_shader(const PUPPY::Shader* shader) {

    auto texture = layer->get_texture()->get_texture();
    Vec2f size = (Vec2f) layer->get_size();

    Vertex vertices[] = {
        {{0, 0}, {0, 1}},
        {{0, 1}, {0, 0}},
        {{1, 1}, {1, 0}},
        {{1, 0}, {1, 1}}
    };

    for(int i = 0; i < 4; i++) {
        vertices[i].position *= size;
        vertices[i].shape_position *= size;
    }

    fuse_context({255, 255, 255, 255}, PUPPY::RenderMode(PUPPY::COPY, shader));
    shared_fill_style.get_render_states()->texture = texture;
    plugin->host_interface->get_drawing_context()->fill_shape(vertices, 4, PrimitiveType::triangle_fan);
    shared_fill_style.get_render_states()->texture = nullptr;
    unfuse_context();
}

void HostRenderTarget::unfuse_context() {
    auto* context = plugin->host_interface->get_drawing_context();
    context->pop_render_target();

    if(layer) layer->set_needs_redraw();
}

void HostRenderTarget::fuse_context(const PUPPY::RGBA &color, const PUPPY::RenderMode &mode) {
    setup_render_mode(mode);

    shared_fill_style.set_color(HostAppInterface::prgba_to_vec4f(color));
    plugin->host_interface->get_drawing_context()->set_fill_style(&shared_fill_style);
}

void HostRenderTarget::setup_render_mode(const PUPPY::RenderMode &mode) {
    auto* context = plugin->host_interface->get_drawing_context();

    context->push_render_target(target);

    if(mode.blend == PUPPY::ALPHA_BLEND) {
        shared_fill_style.get_render_states()->blendMode = sf::BlendAlpha;
    } else {
        shared_fill_style.get_render_states()->blendMode = sf::BlendNone;
    }

    const HostAppShader* host_shader = dynamic_cast<const HostAppShader*>(mode.shader);

    if(host_shader) {
        shared_fill_style.get_render_states()->shader = host_shader->get_shader();
    } else {
        shared_fill_style.get_render_states()->shader = nullptr;
    }
}

void HostRenderTarget::set_texture(DrawingTargetTexture* p_texture) {
    target = p_texture;
}
