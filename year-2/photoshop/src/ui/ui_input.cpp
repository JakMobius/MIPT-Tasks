//
// Created by Артем on 25.10.2021.
//

#include "ui_input.hpp"

static UIInputStyle UIInputStyleInstance {};
UIInputStyle* UIInputStyle::instance = &UIInputStyleInstance;

void UIInput::focus() {
    UIView::focus();
    update_style();
    enable_cursor_blinking();
}

void UIInput::blur() {
    UIView::blur();
    if(enter_callback) enter_callback();
    update_style();
    disable_cursor_blinking();
}

void UIInput::on_key_up(KeyUpEvent* event) {
    UIView::on_key_up(event);
}

void UIInput::on_key_down(KeyDownEvent* event) {
    UIView::on_key_down(event);

    switch(event->code) {
        case KeyCode::left: move_cursor_left(); break;
        case KeyCode::right: move_cursor_right(); break;
        case KeyCode::enter: blur(); break;
        default: break;
    }
}

void UIInput::layout() {
    if(!style) set_style(UIInputStyle::instance);
    UIView::layout();
    text_drawer.set_size(text_insets.get_nested_size(size));
}

void UIInput::set_style(const UIInputStyle* p_style) {
    Styled::set_style(p_style);
    update_style();
}

void UIInput::update_style() {
    if(focused) {
        set_fill_style(style->get_focused_background_color());
    } else {
        set_fill_style(style->get_background_color());
    }
    text_drawer.set_font_color(style->get_text_color());
}

void UIInput::on_text_enter(TextEnterEvent* event) {
    UIView::on_text_enter(event);

    if(!event->is_ascii()) return;

    char c = event->get_ascii();

    if(c == '\r' || c == '\n') return;
    else if(c == '\b') {
        handle_backspace();
    } else {
        contents.insert(contents.begin() + cursor_position, event->get_ascii());
        cursor_position++;
    }

    update_text();
}

void UIInput::handle_backspace() {
    if(cursor_position > 0) {
        cursor_position--;
        contents.erase(contents.begin() + cursor_position);
    }
}

void UIInput::update_text() {
    if(cursor_position >= contents.size()) cursor_position = contents.size() - 1;
    text_drawer.set_text(&contents[0]);
    if(change_callback) change_callback();
    set_needs_redraw();
}

void UIInput::draw(DrawingContext* ctx) {
    UIView::draw(ctx);
    text_drawer.draw(ctx, text_insets.get_nested_position(size));
    if(cursor_visible) {
        ctx->set_fill_style(style->get_cursor_fill_style());
        Vec2f char_position = text_drawer.get_char_position(cursor_position);
        ctx->fill_rect(char_position, {2, (float)text_drawer.get_font_size()});
    }
}

void UIInput::blink() {
    DispatchQueue::main.cancel(blink_task_handle);
    if(!focused) return;

    cursor_visible = !cursor_visible;
    set_needs_redraw();

    blink_task_handle = DispatchQueue::main.push(DispatchQueueTask { [this]() {
        blink();
    }, 500});
}

void UIInput::enable_cursor_blinking() {
    cursor_visible = false;
    blink();
}

void UIInput::disable_cursor_blinking() {
    cursor_visible = false;
    DispatchQueue::main.cancel(blink_task_handle);
}

void UIInput::move_cursor_left() {
    if(cursor_position > 0) cursor_position--;
    enable_cursor_blinking();
}

void UIInput::move_cursor_right() {
    if(cursor_position < contents.size() - 1) cursor_position++;
    enable_cursor_blinking();
}
