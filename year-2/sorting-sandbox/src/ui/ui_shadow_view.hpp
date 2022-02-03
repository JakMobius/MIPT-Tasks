#pragma once

#include "ui_view.hpp"
#include "../app/assets.hpp"
#include "styles/fill_style/fill_style_texture.hpp"

class UIShadowView: public UIView {
public:
    UIShadowView(): UIView() {}

    void draw(DrawingContext *ctx) override;

    void layout() override {
        UIView::layout();
    }
};