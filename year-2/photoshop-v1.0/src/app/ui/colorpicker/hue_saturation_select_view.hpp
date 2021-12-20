#pragma once

#include "../../../ui/ui_view.hpp"
#include "../../../graphics/texture.hpp"
#include "../../../utils/vec3.hpp"
#include "../../../utils/hsv.hpp"
#include "../../assets.hpp"
#include "../../../ui/styles/fill_style/fill_style_texture.hpp"

class HueSaturationSelectView : public UIView {
    Texture* hue_saturation_map = nullptr;
    UIFillStyleTexture texture_fill_style {};
    UIFillStyleTexture cursor_style {};
    UIView* selector = nullptr;
    std::function<void(float, float)> callback;

    void create_texture();
    void handle_mouse(Vec2f position);

public:
    explicit HueSaturationSelectView(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}): UIView(position, size) {
        selector = new UIView({0, 0}, {16, 16});
        cursor_style.set_texture(Assets.hue_select_cursor);
        selector->set_fill_style(&cursor_style);
        selector->set_interactions_enabled(false);
        create_texture();
        append_child(selector);
    }

    void layout() override {
        UIView::layout();
        if(!fill_style) set_fill_style(&texture_fill_style);
    }

    void on_mouse_down(MouseDownEvent *event) override;
    void on_mouse_move(MouseMoveEvent *event) override;

    void set_value(float hue, float saturation);

    void set_callback(const std::function<void(float, float)>& p_callback) { callback = p_callback; }

    void set_hue(float hue);
};