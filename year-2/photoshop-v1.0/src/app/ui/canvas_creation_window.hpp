#pragma once

#include "../../ui/window/ui_window.hpp"
#include "../../ui/ui_input.hpp"
#include "photoshop_window.hpp"
#include "alert_window.hpp"
#include "dialog_window.hpp"

class CanvasCreationWindow: public DialogWindow {

    std::function<void(long, long)> callback {};
    UINumberInput* width_input = nullptr;
    UINumberInput* height_input = nullptr;
    UIButton* create_button = nullptr;

    static bool try_parse_size(const char* string, long* result);
    void try_create_canvas();

public:

    explicit CanvasCreationWindow(PhotoshopView* app);
    void set_callback(const std::function<void(long, long)>& p_callback) { callback = p_callback; }
};