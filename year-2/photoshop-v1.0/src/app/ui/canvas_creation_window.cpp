//
// Created by Артем on 09.11.2021.
//

#include "canvas_creation_window.hpp"

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
    int width = (int)width_input->get_float_value();
    int height = (int)height_input->get_float_value();

    if(width < 0) present_alert("Canvas width cannot be negative");
    else if(height < 0) present_alert("Canvas height cannot be negative");
    else if(width > 512) present_alert("Canvas width cannot be greater\nthan 512px");
    else if(height > 512) present_alert("Canvas height cannot be greater\nthan 512px");
    else {
        if(callback) callback(width, height);
        close();
    }
}

CanvasCreationWindow::CanvasCreationWindow(PhotoshopView* app) : DialogWindow(app, {}, {300, 156}, "New canvas") {

    width_input = create_number_field("Width");
    height_input = create_number_field("Height");

    height_input->set_number_format("%.f");
    width_input->set_number_format("%.f");

    width_input->set_max(512);
    height_input->set_max(512);

    width_input->set_float_value(300);
    height_input->set_float_value(300);

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
