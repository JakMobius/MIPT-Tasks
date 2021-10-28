#pragma once

#include <SFML/Graphics/Font.hpp>
#include "../graphics/drawable_texture.hpp"

struct AssetsStructure {
private:
    std::vector<DrawableTexture*> owned_textures;
    DrawableTexture* own_texture(DrawableTexture* texture);
public:

    DrawableTexture* tool_brush_texture = nullptr;
    DrawableTexture* tool_rubber_texture = nullptr;
    DrawableTexture* tool_pipette_texture = nullptr;
    DrawableTexture* canvas_background_texture = nullptr;
    DrawableTexture* hue_select_cursor = nullptr;
    DrawableTexture* window_close_icon = nullptr;
    DrawableTexture* window_close_hover_icon = nullptr;
    DrawableTexture* window_close_clicked_icon = nullptr;
    DrawableTexture* window_close_inactive_icon = nullptr;
    DrawableTexture* window_shadow_texture = nullptr;
    DrawableTexture* window_shadow_horizontal_texture = nullptr;
    DrawableTexture* window_shadow_corner_texture = nullptr;
    DrawableTexture* slider_bar_texture = nullptr;
    DrawableTexture* window_border_corner_texture = nullptr;
    DrawableTexture* window_border_horizontal_texture = nullptr;
    DrawableTexture* window_border_vertical_texture = nullptr;
    sf::Font* default_font = nullptr;

    void load();
    void unload();

};

extern AssetsStructure Assets;