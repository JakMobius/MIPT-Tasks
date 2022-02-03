//
// Created by Артем on 08.09.2021.
//

#include "user-controller.hpp"

void UserController::handle_event(const sf::Event &event)  {
    switch(event.type) {
        case sf::Event::Closed: {
            window->close();
            root_view->blur();
            break;
        }
        case sf::Event::TextEntered: {
            TextEnterEvent text_event(event.text.unicode);
            if(root_view) root_view->on_text_enter(&text_event);
            break;
        }
        case sf::Event::KeyPressed: {
            KeyDownEvent key_down_event((KeyCode)(event.key.code));
            key_down_event.m_modifiers = get_modifiers();

            if(event.key.code == sf::Keyboard::LShift) shift_pressed = true;
            if(event.key.code == sf::Keyboard::LSystem) cmd_pressed = true;
            if(event.key.code == sf::Keyboard::LAlt) alt_pressed = true;
            if(event.key.code == sf::Keyboard::LControl) ctrl_pressed = true;

            if(root_view) root_view->on_key_down(&key_down_event);
            break;
        }
        case sf::Event::KeyReleased: {
            KeyUpEvent key_up_event((KeyCode)(event.key.code));

            if(event.key.code == sf::Keyboard::LShift) shift_pressed = false;
            if(event.key.code == sf::Keyboard::LSystem) cmd_pressed = false;
            if(event.key.code == sf::Keyboard::LAlt) alt_pressed = false;
            if(event.key.code == sf::Keyboard::LControl) ctrl_pressed = false;

            if(root_view) root_view->on_key_up(&key_up_event);
            break;
        }
        case sf::Event::MouseEntered: {
            MouseInEvent mouse_event((float) event.mouseMove.x, (float) event.mouseMove.y);
            mouse_event.m_modifiers = get_modifiers();
            if(root_view) root_view->on_mouse_in(&mouse_event);
            break;
        }
        case sf::Event::MouseLeft: {
            MouseOutEvent mouse_event((float) event.mouseMove.x, (float) event.mouseMove.y);
            mouse_event.m_modifiers = get_modifiers();
            if(root_view) root_view->on_mouse_out(&mouse_event);
            break;
        }
        case sf::Event::MouseMoved: {
            MouseMoveEvent mouse_event((float) event.mouseMove.x, (float) event.mouseMove.y);
            mouse_event.m_modifiers = get_modifiers();
            if(root_view) root_view->on_mouse_move(&mouse_event);
            break;
        }
        case sf::Event::MouseButtonPressed: {
            if(event.mouseButton.button != sf::Mouse::Left) break;
            MouseDownEvent mouse_event((float) event.mouseButton.x, (float) event.mouseButton.y);
            mouse_event.m_modifiers = get_modifiers();
            if(root_view) root_view->on_mouse_down(&mouse_event);
            break;
        }
        case sf::Event::MouseButtonReleased: {
            if(event.mouseButton.button != sf::Mouse::Left) break;
            MouseUpEvent mouse_event((float) event.mouseButton.x, (float) event.mouseButton.y);
            mouse_event.m_modifiers = get_modifiers();
            if(root_view) root_view->on_mouse_up(&mouse_event);
            break;
        }
        case sf::Event::MouseWheelScrolled: {

            float delta = event.mouseWheelScroll.delta;
            if(delta == 0) break;

            if(event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
                MouseScrollEvent mouse_event((float)event.mouseWheelScroll.x, (float)event.mouseWheelScroll.y, delta, 0);
                mouse_event.m_modifiers = get_modifiers();
                if(root_view) root_view->on_mouse_scroll(&mouse_event);
            } else {
                MouseScrollEvent mouse_event((float)event.mouseWheelScroll.x, (float)event.mouseWheelScroll.y, 0, delta);
                mouse_event.m_modifiers = get_modifiers();
                if(root_view) root_view->on_mouse_scroll(&mouse_event);
            }

            break;
        }
        default: break;
    }
}

KeyModifiers UserController::get_modifiers() {
    return {
        shift_pressed,
        cmd_pressed,
        alt_pressed,
        ctrl_pressed,
    };
}
