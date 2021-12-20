#pragma once

class RawHostWidget;
class HostRenderTarget;

#include "lib_std/widget_std.h"
#include "../../ui/ui_view.hpp"
#include "../../ui/styles/fill_style/fill_style_texture.hpp"
#include "proxy_view.hpp"
#include "plugin.hpp"

class RawHostWidget : virtual public PUPPY::Widget {

protected:
    PUPPY::WBody body {};
    PUPPY::RenderTarget* render_target = nullptr;
    RawHostWidget* parent {};
    UIFillStyleTexture* fill_style {};
    UIView* view {};
    Plugin* owner = nullptr;
    PUPPY::Widget* captured_child = nullptr;

    std::vector<PUPPY::Widget*> children {};

    void set_view(UIView* p_view);

public:

    RawHostWidget(const PUPPY::WBody& body, PUPPY::Widget* parent);

    Plugin* get_owner() { return owner; }
    void set_owner(Plugin* p_owner) { owner = p_owner; }

    UIView* get_view() const { return view; }

    void set_position(const PUPPY::Vec2f &position_) override;

    void set_size(const PUPPY::Vec2f &size_) override;

    PUPPY::WBody get_body() override;

    void set_body(const PUPPY::WBody &body_) override;

    PUPPY::Widget* get_parent() const override;

    void set_parent(Widget* parent_) override;

    PUPPY::RenderTarget* get_texture() override;

    void set_texture(PUPPY::RenderTarget* texture_) override;

    bool is_active() override;

    bool is_inside(const PUPPY::Vec2f &pos) override;

    bool add_child(Widget* child) override;

    void set_to_delete() override;

    bool delete_child(Widget* child) override;

    bool delete_from_parent() override;

    void on_render(const PUPPY::Event::Render &event) override;

    void on_tick(const PUPPY::Event::Tick &event) override;

    void on_mouse_press(const PUPPY::Event::MousePress &event) override;

    void on_mouse_release(const PUPPY::Event::MouseRelease &event) override;

    void on_mouse_move(const PUPPY::Event::MouseMove &event) override;

    void on_key_down(const PUPPY::Event::KeyDown &event) override;

    void on_key_up(const PUPPY::Event::KeyUp &event) override;

    void on_text_enter(const PUPPY::Event::TextEnter &event) override;

    void on_scroll(const PUPPY::Event::Scroll &event) override;

    void on_hide(const PUPPY::Event::Hide &event) override;

    void on_show(const PUPPY::Event::Show &event) override;

    void hide() override;

    void show() override;

    void focus() override;

    void set_caption(const char* text, size_t font_size, const PUPPY::Vec2f* pos) override;

    void set_base_color(const PUPPY::RGBA &color) override;

    static void draw_internal(DrawingContext* ctx, PUPPY::Widget* widget, bool shift);
    static void draw_texture(DrawingContext* ctx, Widget* widget, bool shift, const HostRenderTarget* host_render_target);

    const std::vector<Widget*>& get_children() {
        return children;
    }

    void* get_extra_data(void* arg) override;
};
