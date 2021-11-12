#pragma once

#include "../../ui/window/ui_window.hpp"
#include "../../ui/ui_input.hpp"
#include "photoshop_window.hpp"
#include "alert_window.hpp"

class CanvasCreationWindow: public PhotoshopWindow {

    std::function<void(long, long)> callback {};
    UIStackView* stack = nullptr;
    UIInput* width_input = nullptr;
    UIInput* height_input = nullptr;
    UIButton* create_button = nullptr;

    UIInput* create_field(const char* title);
    void present_alert(const char* reason);
    static bool try_parse_size(const char* string, long* result);
    void try_create_canvas();

public:

    explicit CanvasCreationWindow(PhotoshopView* app);
    void set_callback(const std::function<void(long, long)>& p_callback) { callback = p_callback; }
};