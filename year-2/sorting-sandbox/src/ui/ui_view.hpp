#pragma once

class UIView;
class UIScreen;

#include "../events/mouse-events.hpp"
#include "../events/event-emitter.hpp"
#include "../utils/vec2.hpp"
#include "../utils/matrix3.hpp"
#include "../utils/vec4.hpp"
#include "styles/fill_style/fill_style.hpp"
#include "../graphics/drawing-context.hpp"
#include "../graphics/shapes/shape.hpp"
#include "../events/keyboard-events.hpp"
#include "styles/fill_style/fill_style_color.hpp"
#include <vector>
#include <climits>
#include <cassert>
#include <functional>

extern const UIFillStyleColor UIViewWhiteBackground;
extern const UIFillStyleColor UIViewRedBackground;

struct ViewDestroyEvent {
    UIView* view;
};

class UIView {

protected:
    Vec2f m_position;
    Vec2f m_size;
    Matrix3f m_transform {};
    Matrix3f m_inv_transform {};

    std::vector<UIView*> m_children {};
    UIView* m_current_hovered_child = nullptr;
    UIView* m_current_clicked_child = nullptr;
    UIView* m_current_focused_child = nullptr;
    UIView* m_parent = nullptr;

    DrawingTargetTexture* m_texture = nullptr;
    Shape* m_shape = nullptr;

    const UIFillStyle* m_fill_style = nullptr;

    EventEmitter<ViewDestroyEvent> m_destroy_event_emitter {};

    /* Preferences */
    bool m_masks_to_bounds = false;
    bool m_interactions_enabled = true;
    bool m_cache_texture = true;
    bool m_active = true;
    bool m_focusable = false;

    /* State flags */
    bool m_needs_texture_decision = true;
    bool m_needs_children_layout = true;
    bool m_needs_layout = true;
    bool m_texture_valid = false;
    bool m_needs_redraw = true;
    bool m_hidden = false;
    bool m_hovered = false;
    bool m_clicked = false;
    bool m_focused = false;
    bool m_wrap_focus = false;
    bool m_destroyed = false;

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
    virtual void handle_child_blur();

    bool focus_next_upwards();
    bool focus_previous_upwards();

    virtual ~UIView();

public:

    explicit UIView(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}): m_position(position), m_size(size) {}

    virtual void destroy();

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

    const Vec2f& get_position() { return m_position; }
    virtual void set_position(const Vec2f& pos) { m_position = pos; set_needs_redraw(); }

    const Vec2f& get_size() { return m_size; }
    virtual void set_size(const Vec2f& new_size);

    virtual void layout();
    void layout_if_needed();
    void set_needs_layout();

    int get_child_index(UIView* child);

    bool get_needs_layout() const { return m_needs_layout; };
    bool get_needs_children_layout() const { return m_needs_children_layout; };

    void set_needs_redraw();
    void set_needs_texture_update();
    bool get_needs_redraw() const;

    bool get_hidden() const { return m_hidden; }
    void set_hidden(bool p_hidden) { m_hidden = p_hidden; }

    bool get_is_focused() const { return m_focused; }
    virtual void focus();
    virtual void blur();

    bool focus_first_child(int start_index = 0, int end_index = INT_MAX);
    bool focus_last_child(int start_index = INT_MAX, int end_index = 0);

    bool focus_next();
    bool focus_previous();

    bool get_active() const { return m_active; };
    virtual void set_active(bool p_is_active);

    bool get_masks_to_bounds() const { return m_masks_to_bounds; }
    void set_masks_to_bounds(bool p_masks_to_bounds) { m_masks_to_bounds = p_masks_to_bounds; m_needs_texture_decision = true; }

    bool get_cache_texture() const { return m_cache_texture; }
    void set_cache_texture(bool p_cache_texture) { m_cache_texture = p_cache_texture; }

    Shape* get_shape() const;
    void set_shape(Shape* p_shape);

    const UIFillStyle* get_fill_style() { return m_fill_style; }
    void set_fill_style(const UIFillStyle* new_fill_style, float animation_duration = 0);

    const Matrix3f& get_transform() { return m_transform; }
    const Matrix3f& get_inv_transform() { return m_inv_transform; };
    virtual void set_transform(const Matrix3f& new_transform);;

    bool get_is_clicked() const { return m_clicked; };
    bool get_hovered() const { return m_hovered; };

    bool get_interactions_enabled() const { return m_interactions_enabled; }
    virtual void set_interactions_enabled(bool p_interactions_enabled) { m_interactions_enabled = p_interactions_enabled; }

    /// Returns local coordinates of point in parent coordinates
    Vec2f get_local_position(const Vec2f& external_position);

    /// Returns parent coordinates of point in local coordinates
    Vec2f get_parent_position(const Vec2f& local_position);

    /// Returns screen coordinates of point in local coordinates
    Vec2f get_screen_position(Vec2f local_position);

    const std::vector<UIView*>& get_children() const { return m_children; }
    void append_child(UIView* child);
    void remove_child(int index);
    UIView* get_parent() const { return m_parent; }

    virtual UIScreen* get_screen();

    UIView* test(const Vec2f &point, Vec2f* internal_point) const;

    EventEmitter<ViewDestroyEvent>* get_destroy_event_emitter() { return &m_destroy_event_emitter; }
};