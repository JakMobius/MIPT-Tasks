#pragma once

#include <SFML/Graphics/Font.hpp>
#include "../graphics/drawable_texture.hpp"

struct AssetsStructure {

DrawableTexture* tool_brush_texture = nullptr;
DrawableTexture* tool_rubber_texture = nullptr;
DrawableTexture* tool_pipette_texture = nullptr;
sf::Font* default_font = nullptr;

void load();
void unload();

};

extern AssetsStructure Assets;