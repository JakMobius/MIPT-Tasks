#pragma once

class UIView;

#include "../utils/vec2.hpp"
#include "../utils/dynamic_array.hpp"
#include "../utils/matrix3.hpp"
#include "../utils/vec3.hpp"
#include "../drawing_context.hpp"
#include "../utils/vec4.hpp"
#include "../events/mouse_events.hpp"

class UIView {

private:
    UIView* current_hovered_child = nullptr;
    UIView* current_clicked_child = nullptr;

protected:
    Vec2d position;
    Matrix3d transform;
    Matrix3d inv_transform;
    Vec4d background;
    dynamic_array<UIView*> children;

    void transform_context(DrawingContext* ctx);

    Vec2d size;
public:

    UIView(Vec2d position, Vec2d size): position(position), size(size), transform(), inv_transform(), background(1, 1, 1, 1), children() {}
    virtual ~UIView() {};

    virtual void draw(DrawingContext* ctx);
    virtual void on_mouse_in(MouseInEvent *event);
    virtual void on_mouse_move(MouseMoveEvent *event);
    virtual void on_mouse_out(MouseOutEvent *event);
    virtual void on_mouse_down(MouseDownEvent *event);
    virtual void on_mouse_up(MouseUpEvent *event);

    const Vec2d& get_position() { return position; }
    void set_position(const Vec2d& pos) { position = pos; }

    const Vec2d& get_size() { return size; }
    void set_size(const Vec2d& new_size) { size = new_size; }

    const Vec4d& get_background() { return background; }
    void set_background(const Vec4d& new_background) { background = new_background; }

    const Matrix3d& get_transform() { return transform; }
    void set_transform(const Matrix3d& new_transform) { transform = new_transform; inv_transform = transform.inverse(); }

    Vec2d get_local_position(const Vec2d& external_position);

    void append_child(UIView* child) { children.push(child); }

    const dynamic_array<UIView*>& get_children() const { return children; }

    const Matrix3d& get_inv_transform() { return inv_transform; };
};