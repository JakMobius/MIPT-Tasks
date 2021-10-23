#pragma once

#include <SFML/Graphics/Font.hpp>
#include "../graphics/drawable_texture.hpp"

struct AssetsStructure {

DrawableTexture* tool_brush_texture = nullptr;
DrawableTexture* tool_rubber_texture = nullptr;
DrawableTexture* tool_pipette_texture = nullptr;
DrawableTexture* canvas_background_texture = nullptr;
DrawableTexture* hue_select_cursor = nullptr;
sf::Font* default_font = nullptr;

void load();
void unload();

};

extern AssetsStructure Assets;