#pragma once

class UIModalViewPresentMode;
class UIModalView;

#include "ui_view.hpp"
#include "ui_modal_container.hpp"

class UIModalViewPresentMode {
public:
    virtual void locate(UIModalContainerView* container, UIModalView* view) const = 0;
    virtual ~UIModalViewPresentMode() {};
};

class UIModalViewPresentModeOverlay : public UIModalViewPresentMode {
public:
    void locate(UIModalContainerView* container, UIModalView* view) const override;
};

class UIModalViewPresentModeDropdown : public UIModalViewPresentMode {
    UIView* parent;
public:
    UIModalViewPresentModeDropdown(UIView* parent): parent(parent) {};

    void locate(UIModalContainerView* container, UIModalView* view) const override;
};

class UIModalView : public UIView {
protected:
    UIModalContainerView* container = nullptr;
    UIModalViewPresentMode* present_mode = nullptr;
public:
    explicit UIModalView(const Vec2f& size = {}): UIView({}, size) {};
    ~UIModalView() { delete present_mode; }

    const UIModalViewPresentMode* get_present_mode() const { return present_mode; }

    void set_container(UIModalContainerView* p_container) { container = p_container; }
    UIModalContainerView* get_container() const { return container; }
};

class UIModalViewOverlay : public UIModalView {

    static UIFillStyleColor default_background;
    UIView* stretched_child = nullptr;
public:
    UIModalViewOverlay(): UIModalView() {
        present_mode = new UIModalViewPresentModeOverlay();
        set_fill_style(&default_background);
    }

    void set_stretched_child(UIView* view);
    void on_mouse_click(MouseClickEvent* event) override;
    void layout() override;
};