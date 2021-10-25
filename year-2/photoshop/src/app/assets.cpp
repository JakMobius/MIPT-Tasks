//
// Created by Артем on 21.10.2021.
//

#include "assets.hpp"

void AssetsStructure::load() {
    tool_brush_texture                  = own_texture(new DrawableTexture("resources/img/tools/brush.png"));
    tool_rubber_texture                 = own_texture(new DrawableTexture("resources/img/tools/rubber.png"));
    tool_pipette_texture                = own_texture(new DrawableTexture("resources/img/tools/pipette.png"));
    hue_select_cursor                   = own_texture(new DrawableTexture("resources/img/hue_select_cursor.png"));
    window_close_icon                   = own_texture(new DrawableTexture("resources/img/window-close.png"));
    window_close_clicked_icon           = own_texture(new DrawableTexture("resources/img/window-close-clicked.png"));
    window_close_hover_icon             = own_texture(new DrawableTexture("resources/img/window-close-hover.png"));
    window_close_inactive_icon          = own_texture(new DrawableTexture("resources/img/window-close-inactive.png"));
    window_shadow_texture               = own_texture(new DrawableTexture("resources/img/window-shadow.png"));
    window_shadow_horizontal_texture    = own_texture(new DrawableTexture("resources/img/window-shadow-horizontal.png"));
    window_shadow_corner_texture        = own_texture(new DrawableTexture("resources/img/window-shadow-corner.png"));
    canvas_background_texture           = own_texture(new DrawableTexture("resources/img/canvas_background.png"));
    slider_bar_texture                  = own_texture(new DrawableTexture("resources/img/slider-bar.png"));
    canvas_background_texture->set_repeating(true);
    default_font = new sf::Font();
    default_font->loadFromFile("resources/font/font.ttf");
}

void AssetsStructure::unload() {
    for(int i = 0; i < owned_textures.size(); i++) {
        delete owned_textures[i];
    }
    owned_textures.clear();
    delete default_font;
}

DrawableTexture* AssetsStructure::own_texture(DrawableTexture* texture) {
    owned_textures.push_back(texture);
    return texture;
}

AssetsStructure Assets {};
