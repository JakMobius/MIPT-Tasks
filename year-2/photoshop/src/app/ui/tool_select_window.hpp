#pragma once

#include "../../ui/window/ui_window.hpp"
#include "../../graphics/drawable_texture.hpp"
#include "../assets.hpp"

struct ToolButtonStyle : public UIButtonStyle {
    UIFillStyleTexture* style;
    explicit ToolButtonStyle(UIFillStyleTexture* style): style(style) {}
    ~ToolButtonStyle() override { delete style; }
    const UIFillStyle* get_idle_color() const override { return style; };
};

class ToolSelectWindow : public UIWindow {
    UIStackView* stack = nullptr;

    void create_tool_buttons() {
        for(int i = 0; i < 10; i++) {
            auto* button = new UIButton({}, {50, 50});
            auto* texture = Assets.tool_brush_texture;
            auto* fill_style = new UIFillStyleTexture(texture);
            auto* style = new ToolButtonStyle(fill_style);
            button->set_style(style);
            stack->append_child(button);

            fill_style->set_scale({512.f / 50.f, 512.f / 50.f});
        }
    }

public:
    explicit ToolSelectWindow(const Vec2f& position): UIWindow(position, {}, "") {
        stack = new UIStackView(UIStackViewDirection::y);
        stack->set_primary_alignment(UIStackViewPrimaryAlignment::leading);
        stack->set_item_spacing(7);
        stack->set_insets({7});
        stack->set_fitting({});

        stack->set_fill_style(&UIViewWhiteBackground);

        create_tool_buttons();

        get_content_view()->append_child(stack);
    };

    void layout() override {
        stack->layout_if_needed();
        get_content_view()->set_size(stack->get_size());
        UIWindow::layout();
    }
};