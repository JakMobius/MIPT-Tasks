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
    handle_enter();
    if(enter_callback) enter_callback();
    update_style();
    disable_cursor_blinking();
}

void UIInput::on_key_up(KeyUpEvent* event) {
    UIView::on_key_up(event);
}

void UIInput::on_key_down(KeyDownEvent* event) {
    UIView::on_key_down(event);

    if(event->is_handled()) return;

    bool shift = event->m_modifiers.shift;
    switch(event->code) {
        case KeyCode::left:
            if(event->m_modifiers.super) {
                jump_to_start(shift);
            } else {
                move_cursor_left(shift);
            }
            event->mark_handled();
            break;
        case KeyCode::right:
            if(event->m_modifiers.super) jump_to_end(shift);
            else move_cursor_right(shift);

            event->mark_handled();
            break;
        case KeyCode::enter: blur(); event->mark_handled(); break;
        case KeyCode::a: if(event->m_modifiers.super) { select_all(); event->mark_handled(); } break;
        case KeyCode::c: if(event->m_modifiers.super) { copy_selection(); event->mark_handled(); } break;
        case KeyCode::v: if(event->m_modifiers.super) { paste_from_clipboard(); event->mark_handled(); } break;
        case KeyCode::x: if(event->m_modifiers.super) { copy_selection(); handle_backspace(); update_text(); event->mark_handled(); } break;
        default: break;
    }
}

void UIInput::layout() {
    if(!style) set_style(UIInputStyle::instance);
    UIView::layout();
    text_drawer.set_size(text_insets.get_nested_size(m_size));
}

void UIInput::set_style(const UIInputStyle* p_style) {
    Styled::set_style(p_style);
    update_style();
}

void UIInput::update_style() {
    if(m_focused) {
        set_fill_style(style->get_focused_background_color());
    } else {
        set_fill_style(style->get_background_color());
    }
    text_drawer.set_font_color(style->get_text_color());
}

void UIInput::on_text_enter(TextEnterEvent* event) {
    UIView::on_text_enter(event);

    if(event->is_handled()) return;
    if(!event->is_ascii()) return;

    char c = event->get_ascii();

    if(c == '\r' || c == '\n' || c == '\t') return;
    else if(c == '\b') {
        handle_backspace();
    } else {
        if(cursor.anchor != -1) handle_backspace();
        contents.insert(contents.begin() + cursor.position, event->get_ascii());
        cursor.position++;
    }

    event->mark_handled();

    update_text();
}

void UIInput::handle_backspace() {
    if(cursor.anchor != -1) {
        int from = cursor.anchor;
        int to = cursor.position;

        if(from > to) {
            from = cursor.position;
            to = cursor.anchor;
        }

        cursor.position = from;
        cursor.anchor = -1;

        contents.erase(contents.begin() + from, contents.begin() + to);
    } else if(cursor.position > 0) {
        cursor.position--;
        contents.erase(contents.begin() + cursor.position);
    }
}

void UIInput::update_text() {
    if(cursor.position >= contents.size()) cursor.position = contents.size() - 1;
    cursor.anchor = -1;
    text_drawer.set_text(&contents[0]);
    if(change_callback) change_callback();
    set_needs_redraw();
}

void UIInput::draw(DrawingContext* ctx) {
    UIView::draw(ctx);
    if(cursor.anchor != -1) {
        ctx->set_fill_style(style->get_selection_color());
        Vec2f anchor_position = text_drawer.get_char_position(cursor.anchor);

        Vec2f cursor_position = text_drawer.get_char_position(cursor.position);
        anchor_position.set_y(anchor_position[1] - 3);
        cursor_position.set_y(cursor_position[1] + (float)text_drawer.get_font_size() + 3);
        cursor_position -= anchor_position;
        ctx->fill_rect(anchor_position, cursor_position);
    }
    text_drawer.draw(ctx, text_insets.get_nested_position(m_size));
    if(cursor_visible) {
        ctx->set_fill_style(style->get_cursor_fill_style());
        Vec2f char_position = text_drawer.get_char_position(cursor.position);
        ctx->fill_rect(char_position, {2, (float)text_drawer.get_font_size()});
    }
}

void UIInput::blink() {
    DispatchQueue::main.cancel(blink_task_handle);
    if(!m_focused) return;

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

void UIInput::move_cursor_left(bool shift) {
    if(shift) {
        if(cursor.position > 0) {
            if(cursor.anchor == -1) cursor.anchor = cursor.position;
            cursor.position--;
            if(cursor.position == cursor.anchor) cursor.anchor = -1;
        }
    } else {
        if(cursor.anchor != -1) {
            if(cursor.anchor < cursor.position) cursor.position = cursor.anchor;
            cursor.anchor = -1;
        } else {
            if(cursor.position > 0) cursor.position--;
        }
    }
    enable_cursor_blinking();
}

void UIInput::move_cursor_right(bool shift) {
    if(shift) {
        if(cursor.position < contents.size() - 1) {
            if(cursor.anchor == -1) cursor.anchor = cursor.position;
            cursor.position++;
            if(cursor.position == cursor.anchor) cursor.anchor = -1;
        }
    } else {
        if(cursor.anchor != -1) {
            if(cursor.anchor > cursor.position) cursor.position = cursor.anchor;
            cursor.anchor = -1;
        } else {
            if(cursor.position < contents.size() - 1) cursor.position++;
        }
    }
    enable_cursor_blinking();
}

void UIInput::set_contents(const char* p_contents) {
    int length = strlen(p_contents);
    contents.resize(length + 1);
    for(int i = 0; i < length; i++) {
        contents[i] = p_contents[i];
    }
    contents[length] = '\0';
    update_text();
}

void UIInput::set_contents(const std::vector<char> &p_contents) {
    contents = p_contents;
    update_text();
}

void UIInput::handle_enter() {
    if(enter_callback) enter_callback();
}

UIInput::~UIInput() {
    if(blink_task_handle != -1) {
        DispatchQueue::main.cancel(blink_task_handle);
    }
}

void UIInput::on_mouse_down(MouseDownEvent* event) {
    UIView::on_mouse_down(event);

    cursor.anchor = -1;
    cursor.position = text_drawer.get_char_index({event->x, event->y});
    enable_cursor_blinking();
}

void UIInput::on_mouse_move(MouseMoveEvent* event) {
    UIView::on_mouse_move(event);
    if(!m_clicked) return;

    auto index = text_drawer.get_char_index({event->x, event->y});
    if(cursor.anchor == -1) {
        if(index == cursor.position) return;
        cursor.anchor = cursor.position;
        cursor.position = index;
        enable_cursor_blinking();
    } else {
        if(index == cursor.anchor) {
            cursor.anchor = -1;
        }
        cursor.position = index;
        enable_cursor_blinking();
    }
}

void UIInput::select_all() {
    cursor.anchor = 0;
    cursor.position = contents.size() - 1;
    if(cursor.position == 0) cursor.anchor = -1;
    enable_cursor_blinking();
}

void UIInput::copy_selection() {
    if(cursor.anchor == -1) return;

    int from = cursor.get_lower();
    int to = cursor.get_upper();

    char* buffer = new char[to - from + 1];
    buffer[to - from] = '\0';
    for(int i = from, j = 0; i < to; i++, j++) {
        buffer[j] = contents[i];
    }

    sf::String string(buffer);
    sf::Clipboard::setString(string);
    delete[] buffer;
}

void UIInput::paste_from_clipboard() {
    int from = cursor.get_lower();
    int to = cursor.get_upper();

    auto copied = sf::Clipboard::getString();
    int copied_length = 0;
    for(int i = 0; i < copied.getSize(); i++) copied_length += ((copied[i] & 0xFF00) == 0);

    char* copied_chars = new char[copied_length];
    for(int i = 0, j = 0; i < copied.getSize(); i++) {
        if((copied[i] & 0xFF00) == 0) copied_chars[j++] = (char)copied[i];
    }

    int move_length = (int)copied_length - to + from;

    int old_size = (int) contents.size();

    if(move_length > 0) {
        contents.resize(contents.size() + move_length);
        for(int i = old_size - 1; i >= to; i--) contents[i + move_length] = contents[i];
    } else {
        for(int i = to; i < old_size; i++) contents[i + move_length] = contents[i];
        contents.resize(contents.size() + move_length);
    }

    for(int i = from, j = 0; j < copied_length; i++, j++) {
        contents[i] = (char)copied_chars[j];
    }

    delete[] copied_chars;

    update_text();
}

void UIInput::jump_to_start(bool shift) {
    int old_position = cursor.position;
    cursor.position = 0;

    if(shift) {
        if(cursor.anchor == -1 && old_position != 0) cursor.anchor = old_position;
    } else {
        cursor.anchor = -1;
    }

    enable_cursor_blinking();
}

void UIInput::jump_to_end(bool shift) {
    int old_position = cursor.position;
    cursor.position = contents.size() - 1;

    if(shift) {
        if(cursor.anchor == -1 && old_position != contents.size() - 1) cursor.anchor = old_position;
    } else {
        cursor.anchor = -1;
    }

    enable_cursor_blinking();
}

int UIInputSelection::get_lower() const {
    if(anchor == -1) return position;
    if(position < anchor) return position;
    return anchor;
}

int UIInputSelection::get_upper() const {
    if(anchor == -1) return position;
    if(position > anchor) return position;
    return anchor;
}
