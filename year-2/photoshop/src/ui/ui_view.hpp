#pragma once

class UIView;
class UIScreen;

#include "../events/mouse_events.hpp"
#include "../utils/vec2.hpp"
#include "../utils/matrix3.hpp"
#include "../utils/vec4.hpp"
#include "../ui/ui_drawing_context.hpp"
#include <vector>

class UIView {

protected:
    Vec2f position;
    Vec2f size;
    Matrix3f transform {};
    Matrix3f inv_transform {};
    Vec4f background {0, 0, 0, 0};
    std::vector<UIView*> children {};
    UIView* current_hovered_child = nullptr;
    UIView* current_clicked_child = nullptr;
    UIView* parent = nullptr;

    bool needs_children_layout = true;
    bool needs_layout = true;
    bool needs_redraw = true;
    bool hidden = false;
    bool hovered = false;
    bool clicked = false;

    void transform_context(UIDrawingContext* ctx);
    bool update_hover(UIView* child, const Vec2f& internal_point);
public:

    explicit UIView(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}): position(position), size(size) {}
    virtual ~UIView();

    virtual void prepare_to_draw(UIDrawingContext* ctx);
    virtual void draw(UIDrawingContext* ctx);
    virtual void on_mouse_in(MouseInEvent *event);
    virtual void on_mouse_move(MouseMoveEvent *event);
    virtual void on_mouse_out(MouseOutEvent *event);
    virtual void on_mouse_down(MouseDownEvent *event);
    virtual void on_mouse_up(MouseUpEvent *event);
    virtual void on_mouse_click(MouseClickEvent *event);

    bool update_hovered_child(float x, float y);

    const Vec2f& get_position() { return position; }
    virtual void set_position(const Vec2f& pos) { position = pos; set_needs_redraw(); }

    const Vec2f& get_size() { return size; }
    virtual void set_size(const Vec2f& new_size);

    virtual void layout();
    void layout_if_needed();
    void set_needs_layout();
    void set_needs_children_layout();

    int get_child_index(UIView* child);

    bool get_needs_layout() const { return needs_layout; };
    bool get_needs_children_layout() const { return needs_children_layout; };

    void set_needs_redraw();
    bool get_needs_redraw() const;

    bool get_hidden() const { return hidden; }
    void set_hidden(bool p_hidden) { hidden = p_hidden; }

    const Vec4f& get_background() { return background; }
    virtual void set_background(const Vec4f& new_background);

    const Matrix3f& get_transform() { return transform; }
    const Matrix3f& get_inv_transform() { return inv_transform; };
    virtual void set_transform(const Matrix3f& new_transform);

    /// Returns local coordinates of point in parent coordinates
    Vec2f get_local_position(const Vec2f& external_position);

    /// Returns parent coordinates of point in local coordinates
    Vec2f get_parent_position(const Vec2f& local_position);

    /// Returns screen coordinates of point in local coordinates
    Vec2f get_screen_position(Vec2f local_position);

    const std::vector<UIView*>& get_children() const { return children; }
    void append_child(UIView* child);
    void remove_child(int index);
    UIView* get_parent() const { return parent; }

    virtual UIScreen* get_screen();

    UIView* test(const Vec2f &point, Vec2f* internal_point) const;
};