//
// Created by Артем on 08.09.2021.
//

#include "user_controller.hpp"

void UserController::handle_event(const sf::Event &event)  {
    if (event.type == sf::Event::Closed) window->close();
    else if(event.type == sf::Event::KeyPressed) {
        switch(event.key.code) {
            case sf::Keyboard::W: w_pressed = true; break;
            case sf::Keyboard::A: a_pressed = true; break;
            case sf::Keyboard::S: s_pressed = true; break;
            case sf::Keyboard::D: d_pressed = true; break;
            case sf::Keyboard::LShift: shift_pressed = true; break;
            case sf::Keyboard::Space: space_pressed = true; break;
            case sf::Keyboard::Up: up_pressed = true; break;
            case sf::Keyboard::Left: left_pressed = true; break;
            case sf::Keyboard::Down: down_pressed = true; break;
            case sf::Keyboard::Right: right_pressed = true; break;
            default: break;
        }
    } else if(event.type == sf::Event::KeyReleased) {
        switch(event.key.code) {
            case sf::Keyboard::W: w_pressed = false; break;
            case sf::Keyboard::A: a_pressed = false; break;
            case sf::Keyboard::S: s_pressed = false; break;
            case sf::Keyboard::D: d_pressed = false; break;
            case sf::Keyboard::LShift: shift_pressed = false; break;
            case sf::Keyboard::Space: space_pressed = false; break;
            case sf::Keyboard::Up: up_pressed = false; break;
            case sf::Keyboard::Left: left_pressed = false; break;
            case sf::Keyboard::Down: down_pressed = false; break;
            case sf::Keyboard::Right: right_pressed = false; break;
            default: break;
        }
    } else if(event.type == sf::Event::MouseEntered) {
        MouseInEvent mouseEvent(event.mouseMove.x, event.mouseMove.y);
        if(root_view) root_view->on_mouse_in(&mouseEvent);
    } else if(event.type == sf::Event::MouseLeft) {
        MouseOutEvent mouseEvent(event.mouseMove.x, event.mouseMove.y);
        if(root_view) root_view->on_mouse_out(&mouseEvent);
    } else if(event.type == sf::Event::MouseMoved) {
        if(old_mouse_x == -1) {
            old_mouse_x = event.mouseMove.x;
            old_mouse_y = event.mouseMove.y;
        }

        MouseMoveEvent mouseEvent(event.mouseMove.x, event.mouseMove.y, event.mouseMove.x - old_mouse_x, event.mouseMove.y - old_mouse_y);
        if(root_view) root_view->on_mouse_move(&mouseEvent);

        old_mouse_x = event.mouseMove.x;
        old_mouse_y = event.mouseMove.y;
    } else if(event.type == sf::Event::MouseButtonPressed) {
        MouseDownEvent mouseEvent(event.mouseButton.x, event.mouseButton.y);
        if(root_view) root_view->on_mouse_down(&mouseEvent);
    } else if(event.type == sf::Event::MouseButtonReleased) {
        MouseUpEvent mouseEvent(event.mouseButton.x, event.mouseButton.y);
        if(root_view) root_view->on_mouse_up(&mouseEvent);
    }
}

void UserController::tick() {

}
