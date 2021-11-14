//
// Created by Артем on 12.11.2021.
//

#include "dialog_window.hpp"
#include "alert_window.hpp"

UIStackView* DialogWindow::add_stack_line() {
    auto* line = new UIStackView(UIAxis::x);
    line->set_fitting({get_content_view()->get_size()[0] - 24, {}});
    line->set_primary_alignment(UIStackViewPrimaryAlignment::space_between);
    stack->append_child(line);
    return line;
}

UIInput* DialogWindow::create_string_field(const char* title) {
    auto* line = add_stack_line();
    auto* input = new UIInput({}, {get_content_view()->get_size()[0] - text_width - 50, 40});
    auto* text = new UIText({}, {50, 40}, title);

    input->get_text_drawer()->set_font_size(17);
    text->get_text_drawer()->set_v_alignment(VTextAlignmentCenter);
    text->get_text_drawer()->set_font_size(17);

    line->append_child(text);
    line->append_child(input);

    return input;
}

UINumberInput* DialogWindow::create_number_field(const char* title) {
    auto* line = add_stack_line();
    auto* input = new UINumberInput({}, {get_content_view()->get_size()[0] - text_width - 50, 40});
    auto* text = new UIText({}, {50, 40}, title);

    input->get_text_drawer()->set_font_size(17);
    text->get_text_drawer()->set_v_alignment(VTextAlignmentCenter);
    text->get_text_drawer()->set_font_size(17);

    line->append_child(text);
    line->append_child(input);

    return input;
}

void DialogWindow::present_alert(const char* reason) {
    this->app->present_window_modally(new AlertWindow(reason));
}

DialogWindow::DialogWindow(PhotoshopView* app, const Vec2f &position, const Vec2f &size, const char* title) : PhotoshopWindow(app, position, size, title) {
    stack = new UIStackView(UIAxis::y);
    stack->set_insets({12, 12});
    stack->set_item_spacing(7);
    stack->set_lateral_alignment(UIStackViewLateralAlignment::trailing);
}

void DialogWindow::on_key_down(KeyDownEvent* event) {
    UIView::on_key_down(event);

    if(!event->is_handled() && event->code == KeyCode::escape) {
        DispatchQueue::main.push(DispatchQueueTask { [this](){
            close();
        }});
    }
}
