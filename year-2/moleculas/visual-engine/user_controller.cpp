//
// Created by Артем on 08.09.2021.
//

#include "user_controller.hpp"

void UserController::handle_event(const sf::Event &event)  {
    if (event.type == sf::Event::Closed) window->close();
    else if(event.type == sf::Event::MouseEntered) {
        MouseInEvent mouseEvent(event.mouseMove.x, event.mouseMove.y);
        root_view->on_mouse_in(&mouseEvent);
    } else if(event.type == sf::Event::MouseLeft) {
        MouseOutEvent mouseEvent(event.mouseMove.x, event.mouseMove.y);
        root_view->on_mouse_out(&mouseEvent);
    } else if(event.type == sf::Event::MouseMoved) {
        if(old_mouse_x == -1) {
            old_mouse_x = event.mouseMove.x;
            old_mouse_y = event.mouseMove.y;
        }

        MouseMoveEvent mouseEvent(event.mouseMove.x, event.mouseMove.y, event.mouseMove.x - old_mouse_x, event.mouseMove.y - old_mouse_y);
        root_view->on_mouse_move(&mouseEvent);

        old_mouse_x = event.mouseMove.x;
        old_mouse_y = event.mouseMove.y;
    } else if(event.type == sf::Event::MouseButtonPressed) {
        MouseDownEvent mouseEvent(event.mouseButton.x, event.mouseButton.y);
        root_view->on_mouse_down(&mouseEvent);
    } else if(event.type == sf::Event::MouseButtonReleased) {
        MouseUpEvent mouseEvent(event.mouseButton.x, event.mouseButton.y);
        root_view->on_mouse_up(&mouseEvent);
    }
}

void UserController::tick() {

}
