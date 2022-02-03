#pragma once

#include <SFML/Graphics/Font.hpp>
#include "../graphics/texture.hpp"

struct AssetsStructure {
private:
public:

    std::unique_ptr<Texture> canvas_background_texture {};
    std::unique_ptr<Texture> hue_select_cursor {};
    std::unique_ptr<Texture> window_close_icon {};
    std::unique_ptr<Texture> window_close_hover_icon {};
    std::unique_ptr<Texture> window_close_clicked_icon {};
    std::unique_ptr<Texture> window_close_inactive_icon {};
    std::unique_ptr<Texture> window_shadow_texture {};
    std::unique_ptr<Texture> window_shadow_horizontal_texture {};
    std::unique_ptr<Texture> window_shadow_corner_texture {};
    std::unique_ptr<Texture> slider_bar_texture {};
    std::unique_ptr<Texture> window_border_corner_texture {};
    std::unique_ptr<Texture> window_border_horizontal_texture {};
    std::unique_ptr<Texture> window_border_vertical_texture {};
    std::unique_ptr<Texture> error_texture {};
    std::unique_ptr<sf::Font> default_font {};

    void load();

};

extern AssetsStructure Assets;