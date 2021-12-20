//
// Created by Артем on 15.12.2021.
//

#include "host_render_target_factory.hpp"
#include "host_render_target.hpp"

PUPPY::RenderTarget* HostRenderTargetFactory::spawn(const PUPPY::Vec2s &size, const PUPPY::RGBA &color) const {
    DrawingTargetTexture* texture = new DrawingTargetTexture(HostAppInterface::pvec2s_to_vec2i(size));
    texture->clear(HostAppInterface::prgba_to_vec4f(color));
    return new HostRenderTarget(texture, interface->get_plugin());
}

PUPPY::RenderTarget* HostRenderTargetFactory::from_pixels(const PUPPY::Vec2s &size, const PUPPY::RGBA* data) const {
    DrawingTargetTexture* texture = new DrawingTargetTexture(HostAppInterface::pvec2s_to_vec2i(size));

    auto target = new HostRenderTarget(texture, interface->get_plugin());
    target->render_pixels(0, size, data, { PUPPY::COPY });
    return target;
}

PUPPY::RenderTarget* HostRenderTargetFactory::from_file(const char* path) const {
    sf::Texture image;
    if(!image.loadFromFile(path)) {
        return nullptr;
    } else {
        sf::Sprite sprite;
        sprite.setTexture(image, true);

        DrawingTargetTexture* texture = new DrawingTargetTexture({(int)image.getSize().x, (int)image.getSize().y});
        texture->get_render_texture()->draw(sprite);
        return new HostRenderTarget(texture, interface->get_plugin());
    }
}
