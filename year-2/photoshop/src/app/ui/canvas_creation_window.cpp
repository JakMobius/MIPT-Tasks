//
// Created by Артем on 09.11.2021.
//

#include "canvas_creation_window.hpp"

void CanvasCreationWindow::present_alert(const char* reason) {
    this->app->present_window_modally(new AlertWindow(reason));
}

bool CanvasCreationWindow::try_parse_size(const char* string, long* result) {
    char* end = nullptr;
    auto value = strtol(string, &end, 10);
    if(*end == '\0') {
        *result = value;
        return true;
    }
    return false;
}

void CanvasCreationWindow::try_create_canvas() {
    const char* width_str = &width_input->get_contents()[0];
    const char* height_str = &height_input->get_contents()[0];

    long width = 0, height = 0;

    if(*width_str == '\0') present_alert("Please, enter canvas width");
    else if(*height_str == '\0') present_alert("Please, enter canvas height");
    else if(!try_parse_size(width_str, &width)) present_alert("Please, enter valid canvas width");
    else if(!try_parse_size(height_str, &height)) present_alert("Please, enter valid canvas height");
    else if(width < 0) present_alert("Canvas width cannot be negative");
    else if(height < 0) present_alert("Canvas height cannot be negative");
    else if(width > 512) present_alert("Canvas width cannot be greater\nthan 512px");
    else if(height > 512) present_alert("Canvas height cannot be greater\nthan 512px");
    else {
        if(callback) callback(width, height);
        close();
    }
}

UIInput* CanvasCreationWindow::create_field(const char* title) {
    auto* line = new UIStackView(UIAxis::x);
    line->set_fitting({276, {}});
    line->set_primary_alignment(UIStackViewPrimaryAlignment::space_between);

    auto* input = new UIInput({}, {200, 40});
    auto* text = new UIText({}, {50, 40}, title);

    input->get_text_drawer()->set_font_size(17);

    text->get_text_drawer()->set_v_alignment(VTextAlignmentCenter);
    text->get_text_drawer()->set_font_size(17);

    line->append_child(text);
    line->append_child(input);

    input->set_contents("300");

    stack->append_child(line);

    return input;
}

CanvasCreationWindow::CanvasCreationWindow(PhotoshopView* app) : PhotoshopWindow(app, {}, {300, 156}, "New canvas") {
    stack = new UIStackView(UIAxis::y);
    stack->set_insets({12, 12});
    stack->set_item_spacing(7);
    stack->set_lateral_alignment(UIStackViewLateralAlignment::trailing);

    width_input = create_field("Width");
    height_input = create_field("Height");

    create_button = new UIButton({}, {80, 40});
    create_button->set_title("Create");
    stack->append_child(create_button);

    create_button->set_callback([this]() {
        DispatchQueue::main.push(DispatchQueueTask {[this]() {
            try_create_canvas();
        }});
    });

    get_content_view()->append_child(stack);
}
