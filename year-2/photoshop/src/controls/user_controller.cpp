//
// Created by Артем on 08.09.2021.
//

#include "user_controller.hpp"

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
            if(event.key.shift) key_down_event.shift = true;
            if(event.key.alt) key_down_event.alt = true;
            if(event.key.system) key_down_event.alt = true;
            if(event.key.control) key_down_event.control = true;
            if(root_view) root_view->on_key_down(&key_down_event);
            break;
        }
        case sf::Event::KeyReleased: {
            KeyUpEvent key_up_event((KeyCode)(event.key.code));
            if(root_view) root_view->on_key_up(&key_up_event);
            break;
        }
        case sf::Event::MouseEntered: {
            MouseInEvent mouse_event((float) event.mouseMove.x, (float) event.mouseMove.y);
            if(root_view) root_view->on_mouse_in(&mouse_event);
            break;
        }
        case sf::Event::MouseLeft: {
            MouseOutEvent mouse_event((float) event.mouseMove.x, (float) event.mouseMove.y);
            if(root_view) root_view->on_mouse_out(&mouse_event);
            break;
        }
        case sf::Event::MouseMoved: {
            MouseMoveEvent mouse_event((float) event.mouseMove.x, (float) event.mouseMove.y);
            if(root_view) root_view->on_mouse_move(&mouse_event);
            break;
        }
        case sf::Event::MouseButtonPressed: {
            if(event.mouseButton.button != sf::Mouse::Left) break;
            MouseDownEvent mouse_event((float) event.mouseButton.x, (float) event.mouseButton.y);
            if(root_view) root_view->on_mouse_down(&mouse_event);
            break;
        }
        case sf::Event::MouseButtonReleased: {
            if(event.mouseButton.button != sf::Mouse::Left) break;
            MouseUpEvent mouse_event((float) event.mouseButton.x, (float) event.mouseButton.y);
            if(root_view) root_view->on_mouse_up(&mouse_event);
            break;
        }
        case sf::Event::MouseWheelScrolled: {

            float delta = event.mouseWheelScroll.delta;
            if(delta == 0) break;

            if(event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
                MouseScrollEvent mouse_event((float)event.mouseWheelScroll.x, (float)event.mouseWheelScroll.y, delta, 0);
                if(root_view) root_view->on_mouse_scroll(&mouse_event);
            } else {
                MouseScrollEvent mouse_event((float)event.mouseWheelScroll.x, (float)event.mouseWheelScroll.y, 0, delta);
                if(root_view) root_view->on_mouse_scroll(&mouse_event);
            }

            break;
        }
        default: break;
    }
}