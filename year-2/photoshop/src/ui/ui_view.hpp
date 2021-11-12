#pragma once

class UIView;
class UIScreen;

#include "../events/mouse_events.hpp"
#include "../utils/vec2.hpp"
#include "../utils/matrix3.hpp"
#include "../utils/vec4.hpp"
#include "styles/fill_style.hpp"
#include "../graphics/drawing_context.hpp"
#include "../graphics/shapes/shape.hpp"
#include "../events/keyboard_events.hpp"
#include <vector>

extern const UIFillStyleColor UIViewWhiteBackground;
extern const UIFillStyleColor UIViewRedBackground;

class UIView {

protected:
    Vec2f position;
    Vec2f size;
    Matrix3f transform {};
    Matrix3f inv_transform {};

    std::vector<UIView*> children {};
    UIView* current_hovered_child = nullptr;
    UIView* current_clicked_child = nullptr;
    UIView* current_focused_child = nullptr;
    UIView* parent = nullptr;

    DrawingTargetTexture* texture = nullptr;
    Shape* shape = nullptr;

    const UIFillStyle* fill_style = nullptr;

    /* Preferences */
    bool masks_to_bounds = false;
    bool interactions_enabled = true;
    bool active = false;

    /* State flags */
    bool needs_texture_decision = true;
    bool needs_children_layout = true;
    bool needs_layout = true;
    bool texture_valid = false;
    bool needs_redraw = true;
    bool hidden = false;
    bool hovered = false;
    bool clicked = false;
    bool focused = false;

    void transform_context(DrawingContext* ctx);
    bool update_hover(UIView* child, const Vec2f& internal_point);
    bool update_hovered_child(float x, float y);

    void decide_whether_to_draw_to_texture();
    void set_draw_to_texture(bool use_texture);
    void recreate_texture();

    void draw_self_and_children(DrawingContext* ctx);
    virtual void draw(DrawingContext* ctx);
    void draw_in_texture(DrawingContext* ctx);
    void draw_without_texture(DrawingContext* ctx);
    void draw_self_texture_shaped(DrawingContext* ctx);

    void set_needs_children_layout();
    void focus_child(UIView* child);
public:

    explicit UIView(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}): position(position), size(size) {}
    virtual ~UIView();

    virtual void prepare_to_draw(DrawingContext* ctx);
    virtual void on_mouse_in(MouseInEvent *event);
    virtual void on_mouse_move(MouseMoveEvent *event);
    virtual void on_mouse_out(MouseOutEvent *event);
    virtual void on_mouse_down(MouseDownEvent *event);
    virtual void on_mouse_up(MouseUpEvent *event);
    virtual void on_mouse_click(MouseClickEvent *event);
    virtual void on_mouse_scroll(MouseScrollEvent *event);
    virtual void on_text_enter(TextEnterEvent *event);
    virtual void on_key_down(KeyDownEvent *event);
    virtual void on_key_up(KeyUpEvent *event);

    const Vec2f& get_position() { return position; }
    virtual void set_position(const Vec2f& pos) { position = pos; set_needs_redraw(); }

    const Vec2f& get_size() { return size; }
    virtual void set_size(const Vec2f& new_size);

    virtual void layout();
    void layout_if_needed();
    void set_needs_layout();

    int get_child_index(UIView* child);

    bool get_needs_layout() const { return needs_layout; };
    bool get_needs_children_layout() const { return needs_children_layout; };

    void set_needs_redraw();
    void set_needs_texture_update();
    bool get_needs_redraw() const;

    bool get_hidden() const { return hidden; }
    void set_hidden(bool p_hidden) { hidden = p_hidden; }

    bool get_is_focused() const { return focused; }
    virtual void focus();
    virtual void blur();

    bool get_masks_to_bounds() const { return masks_to_bounds; }
    void set_masks_to_bounds(bool p_masks_to_bounds) { masks_to_bounds = p_masks_to_bounds; needs_texture_decision = true; }

    Shape* get_shape() const;
    void set_shape(Shape* p_shape);

    const UIFillStyle* get_fill_style() { return fill_style; }
    void set_fill_style(const UIFillStyle* new_fill_style);

    const Matrix3f& get_transform() { return transform; }
    const Matrix3f& get_inv_transform() { return inv_transform; };
    virtual void set_transform(const Matrix3f& new_transform);

    bool get_active() const { return active; };
    virtual void set_active(bool p_is_active);

    bool get_is_clicked() const { return clicked; };
    bool get_hovered() const { return hovered; };

    bool get_interactions_enabled() const { return interactions_enabled; }
    virtual void set_interactions_enabled(bool p_interactions_enabled) { interactions_enabled = p_interactions_enabled; }

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