//
// Created by Артем on 21.10.2021.
//

#include "texture.hpp"

Texture::Texture(const Vec2i &texture_size): size(texture_size) {
    texture = new sf::Texture();
    texture->create(size[0], size[1]);
}

Texture::Texture(const char* path) {
    texture = new sf::Texture();
    texture->loadFromFile(path);
    auto texture_size = texture->getSize();
    size = { (int)texture_size.x, (int)texture_size.y };
}

Texture::Texture(const Vec2i size, const unsigned char* rgba_array): size(size) {
    texture = new sf::Texture();
    texture->create(size[0], size[1]);
    texture->update(rgba_array);
}

Texture::~Texture() {
    delete texture;
}

void Texture::set_repeating(bool is_repeating) {
    texture->setRepeated(is_repeating);
}
