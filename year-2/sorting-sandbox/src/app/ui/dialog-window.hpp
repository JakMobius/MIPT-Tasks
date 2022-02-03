#pragma once

#include "photoshop-window.hpp"
#include "../../ui/ui_input.hpp"
#include "colorpicker/ui_number_input.hpp"

class DialogWindow : public AppWindow {
protected:

    int text_width = 50;

    UIStackView* stack = nullptr;
    void present_alert(const char* reason);
    UIInput* create_string_field(const char* title);
    UINumberInput* create_number_field(const char* title);
    UIStackView* add_stack_line();
    void on_key_down(KeyDownEvent* event);

public:
    explicit DialogWindow(AppView* app, const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}, const char* title = nullptr);

};