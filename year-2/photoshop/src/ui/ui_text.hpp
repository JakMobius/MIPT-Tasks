#pragma once

#include "ui_view.hpp"

class UIText : public UIView {
    const char* str;

    Vec4f text_color = {0, 0, 0, 1};

private:
    HTextAlignment h_text_alignment = HTextAlignmentCenter;
    VTextAlignment v_text_alignment = VTextAlignmentCenter;
public:
    UIText(const Vec2f &position = {0, 0}, const Vec2f &size = {0, 0}, const char* text = nullptr): UIView(position, size), str(text) {

    };
    ~UIText() override = default;

    void draw(DrawingContext* ctx) override;

    HTextAlignment get_h_text_alignment() const { return h_text_alignment; }
    void set_h_text_alignment(HTextAlignment new_h_text_alignment) { h_text_alignment = new_h_text_alignment; }

    VTextAlignment get_v_text_alignment() const { return v_text_alignment; }
    void set_v_text_alignment(VTextAlignment new_v_text_alignment) { v_text_alignment = new_v_text_alignment; }

    void set_text(const char* str) { this->str = str; };
    const char* get_text() { return str; }

    const Vec4f& get_text_color() const { return text_color; }
    void set_text_color(const Vec4f &new_text_color) { text_color = new_text_color; }
};