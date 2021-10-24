#pragma once

class PhotoshopView;

#include "../utils/vec4.hpp"
#include "../ui/ui_view.hpp"
#include "../ui/window/ui_window.hpp"
#include "../ui/window/ui_window_container.hpp"
#include <functional>

class ColorPickerWindow;
class App;

class ColorPickerCloseListener : public EventHandler<WindowCloseEvent> {
    PhotoshopView* view;
public:
    explicit ColorPickerCloseListener(PhotoshopView* view): view(view) {}
    void operator() (WindowCloseEvent* event) override;
};

class PhotoshopView : public UIWindowContainer {

    ColorPickerCloseListener color_picker_close_listener { this };
    ColorPickerWindow* color_piсker = nullptr;
    App* app;

    void create_color_picker();

public:
    explicit PhotoshopView(App* app, const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0});

    void open_colorpicker(const std::function<void(const Vec4f &)> &callback, const Vec4f* current_color);
    void on_color_picker_closed();
};