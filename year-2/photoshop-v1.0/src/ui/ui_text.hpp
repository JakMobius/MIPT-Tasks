#pragma once

#include "ui_view.hpp"

class UIText : public UIView {
    TextDrawer drawer;

public:
    UIText(const Vec2f &position = {0, 0}, const Vec2f &size = {0, 0}, const char* text = nullptr): UIView(position, size), drawer(size, text) {};
    ~UIText() override = default;

    void draw(DrawingContext* ctx) override;

    void layout() override {
        drawer.set_size(size);
        UIView::layout();
    }

    TextDrawer* get_text_drawer() { return &drawer; }
};