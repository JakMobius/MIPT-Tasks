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
    double heading = controlled_camera->get_heading();
    double heading_sin = sin(heading);
    double heading_cos = cos(heading);

    Vec3d position = controlled_camera->get_position();

    double walk_x = ((w_pressed ? 1 : 0) + (s_pressed ? -1 : 0)) * 0.1;
    double walk_y = ((d_pressed ? 1 : 0) + (a_pressed ? -1 : 0)) * 0.1;
    double walk_z = ((shift_pressed ? -1 : 0) + (space_pressed ? 1 : 0)) * 0.1;

    double camera_move_y = ((up_pressed ? 1 : 0) + (down_pressed ? -1 : 0)) * 0.01;
    double camera_move_x = ((right_pressed ? 1 : 0) + (left_pressed ? -1 : 0)) * 0.01;

    position += {walk_x * heading_cos - walk_y * heading_sin, walk_x * heading_sin + walk_y * heading_cos, walk_z};

    double new_heading = fmod(heading + (double)camera_move_x, M_PI * 2);
    double new_pitch = controlled_camera->get_pitch() + (double)camera_move_y;
    if(new_pitch < -M_PI / 2) new_pitch = -M_PI / 2;
    if(new_pitch > M_PI / 2) new_pitch = M_PI / 2;

    controlled_camera->set_heading(new_heading);
    controlled_camera->set_pitch(new_pitch);
    controlled_camera->set_position(position);

}
