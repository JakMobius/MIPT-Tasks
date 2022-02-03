//
// Created by Артем on 21.10.2021.
//

#include "assets.hpp"

void AssetsStructure::load() {
    hue_select_cursor                   = std::make_unique<Texture>("resources/img/hue_select_cursor.png");
    window_close_icon                   = std::make_unique<Texture>("resources/img/window-close.png");
    window_close_clicked_icon           = std::make_unique<Texture>("resources/img/window-close-clicked.png");
    window_close_hover_icon             = std::make_unique<Texture>("resources/img/window-close-hover.png");
    window_close_inactive_icon          = std::make_unique<Texture>("resources/img/window-close-inactive.png");
    window_shadow_texture               = std::make_unique<Texture>("resources/img/window-shadow.png");
    window_shadow_horizontal_texture    = std::make_unique<Texture>("resources/img/window-shadow-horizontal.png");
    window_shadow_corner_texture        = std::make_unique<Texture>("resources/img/window-shadow-corner.png");
    canvas_background_texture           = std::make_unique<Texture>("resources/img/canvas_background.png");
    slider_bar_texture                  = std::make_unique<Texture>("resources/img/slider-bar.png");
    window_border_corner_texture        = std::make_unique<Texture>("resources/img/window-border-corner.png");
    window_border_vertical_texture      = std::make_unique<Texture>("resources/img/window-border-vertical.png");
    window_border_horizontal_texture    = std::make_unique<Texture>("resources/img/window-border-horizontal.png");
    error_texture                       = std::make_unique<Texture>("resources/img/error.png");
    canvas_background_texture->set_repeating(true);
    default_font = std::make_unique<sf::Font>();
    default_font->loadFromFile("resources/font/font.ttf");
}

AssetsStructure Assets {};
