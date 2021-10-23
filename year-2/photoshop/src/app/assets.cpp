//
// Created by Артем on 21.10.2021.
//

#include "assets.hpp"

void AssetsStructure::load() {
    tool_brush_texture = new DrawableTexture("resources/img/tools/brush.png");
    tool_rubber_texture = new DrawableTexture("resources/img/tools/rubber.png");
    tool_pipette_texture = new DrawableTexture("resources/img/tools/pipette.png");
    hue_select_cursor = new DrawableTexture("resources/img/hue_select_cursor.png");
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
    delete default_font;
}

AssetsStructure Assets {};
