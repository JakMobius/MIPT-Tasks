//
// Created by Артем on 21.10.2021.
//

#include "assets.hpp"

void AssetsStructure::load() {
    tool_brush_texture = new DrawableTexture("resources/img/tools/brush.png");
    tool_rubber_texture = new DrawableTexture("resources/img/tools/rubber.png");
    tool_pipette_texture = new DrawableTexture("resources/img/tools/pipette.png");
    hue_select_cursor = new DrawableTexture("resources/img/hue_select_cursor.png");
    window_close_icon = new DrawableTexture("resources/img/window-close.png");
    window_close_clicked_icon = new DrawableTexture("resources/img/window-close-clicked.png");
    window_close_hover_icon = new DrawableTexture("resources/img/window-close-hover.png");
    window_close_inactive_icon = new DrawableTexture("resources/img/window-close-inactive.png");
    window_shadow_texture = new DrawableTexture("resources/img/window-shadow.png");
    window_shadow_horizontal_texture = new DrawableTexture("resources/img/window-shadow-horizontal.png");
    window_shadow_corner_texture = new DrawableTexture("resources/img/window-shadow-corner.png");
    canvas_background_texture = new DrawableTexture("resources/img/canvas_background.png");
    canvas_background_texture->set_repeating(true);
    default_font = new sf::Font();
    default_font->loadFromFile("resources/font/font.ttf");
}

void AssetsStructure::unload() {
    delete tool_brush_texture;
    delete tool_rubber_texture;
    delete tool_pipette_texture;
    delete hue_select_cursor;
    delete canvas_background_texture;
    delete window_close_icon;
    delete window_close_clicked_icon;
    delete window_close_hover_icon;
    delete window_close_inactive_icon;
    delete window_shadow_texture;
    delete window_shadow_horizontal_texture;
    delete window_shadow_corner_texture;
    delete default_font;
}

AssetsStructure Assets {};
