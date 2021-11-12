#pragma once

#include "../../../ui/styles/fill_style.hpp"
#include "catmull_rom_curve.hpp"
#include "../../../graphics/shapes/line_shape.hpp"
#include "../../../ui/ui_view.hpp"

class CatmullRomColorCurveView : public UIView {
    UIFillStyleColor point_color {{0.8, 0.8, 0.8, 1}};
    UIFillStyleColor hovered_point_color {{1, 1, 1, 1}};
    UIFillStyleColor curve_color {{1, 1, 1, 1}};

    std::function<void(void)> callback {};
    CatmullRomCurve curve {};
    LineShape line_shape {};
    int captured_point_index = -1;
    int hovered_point_index = -1;
    Vec2f graph_size {};
    Vec2f graph_position {5, 5};

    // Preferences
    int steps = 25;
    float mouse_capture_distance = 10;
    float min_point_distance = 0.03;
    bool points_visible = true;

    Vec2f graph_position_from_mouse(float x, float y);
    Vec2f mouse_position_from_graph(const Vec2f& position);
    float distance_to_point(const Vec2f& pos, int index);
    void update_hovered_point(float x, float y);
    void handle_mouse_position(float x, float y);
    void points_updated();
public:
    CatmullRomColorCurveView(const Vec2f& position, const Vec2f& size);

    void draw(DrawingContext *ctx) override;

    void on_mouse_move(MouseMoveEvent* event) override;
    void on_mouse_down(MouseDownEvent* event) override;
    void on_mouse_up(MouseUpEvent* event) override;
    void on_mouse_out(MouseOutEvent* event) override;

    void layout() override;
    void set_curve(const CatmullRomCurve& p_curve);
    const CatmullRomCurve& get_curve() { return curve; };
    void set_callback(const std::function<void(void)>& p_callback) { callback = p_callback; }

    void set_curve_color(const Vec4f& color) { curve_color.set_color(color); set_needs_redraw(); }
    Vec4f get_curve_color() { return curve_color.get_color(); }

    void set_hovered_point_color(const Vec4f& color) { hovered_point_color.set_color(color); set_needs_redraw(); }
    Vec4f get_hovered_point_color() { return hovered_point_color.get_color(); }

    void set_point_color(const Vec4f& color) { point_color.set_color(color); set_needs_redraw(); }
    Vec4f get_point_color() { return point_color.get_color(); }

    void set_points_visible(bool b);
};