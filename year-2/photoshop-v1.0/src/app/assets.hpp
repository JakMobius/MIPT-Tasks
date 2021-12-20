#pragma once

#include <SFML/Graphics/Font.hpp>
#include "../graphics/texture.hpp"

struct AssetsStructure {
private:
    std::vector<Texture*> owned_textures;
    Texture* own_texture(Texture* texture);
public:

    Texture* tool_brush_texture = nullptr;
    Texture* tool_rubber_texture = nullptr;
    Texture* tool_pipette_texture = nullptr;
    Texture* canvas_background_texture = nullptr;
    Texture* hue_select_cursor = nullptr;
    Texture* window_close_icon = nullptr;
    Texture* window_close_hover_icon = nullptr;
    Texture* window_close_clicked_icon = nullptr;
    Texture* window_close_inactive_icon = nullptr;
    Texture* window_shadow_texture = nullptr;
    Texture* window_shadow_horizontal_texture = nullptr;
    Texture* window_shadow_corner_texture = nullptr;
    Texture* slider_bar_texture = nullptr;
    Texture* window_border_corner_texture = nullptr;
    Texture* window_border_horizontal_texture = nullptr;
    Texture* window_border_vertical_texture = nullptr;
    Texture* error_texture = nullptr;
    sf::Font* default_font = nullptr;

    void load();
    void unload();

};

extern AssetsStructure Assets;