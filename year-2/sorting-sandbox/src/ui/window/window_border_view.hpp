#pragma once

#include "../../graphics/drawing-context.hpp"
#include "../ui_view.hpp"
#include "../../app/assets.hpp"

class WindowBorderView : public UIView {
public:
    WindowBorderView(): UIView() {}

    void draw(DrawingContext *ctx) override;

    void layout() override {
        UIView::layout();
    }
};