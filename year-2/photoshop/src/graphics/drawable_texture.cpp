//
// Created by Артем on 21.10.2021.
//

#include "drawable_texture.hpp"

DrawableTexture::DrawableTexture(const Vec2i &texture_size): size(texture_size) {
    texture = new sf::Texture();
    texture->create(size[0], size[1]);
}

DrawableTexture::DrawableTexture(const char* path) {
    texture = new sf::Texture();
    texture->loadFromFile(path);
    auto texture_size = texture->getSize();
    size = { (int)texture_size.x, (int)texture_size.y };
}

DrawableTexture::DrawableTexture(const Vec2i size, const unsigned char* rgba_array): size(size) {
    texture = new sf::Texture();
    texture->create(size[0], size[1]);
    texture->update(rgba_array);
}

DrawableTexture::~DrawableTexture() {
    delete texture;
}

void DrawableTexture::set_repeating(bool is_repeating) {
    texture->setRepeated(is_repeating);
}
