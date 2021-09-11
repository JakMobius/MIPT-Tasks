//
// Created by Артем on 08.09.2021.
//

#include "user_controller.hpp"

void UserController::handle_event(const sf::Event &event)  {
    if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::W) w_pressed = true;
        if(event.key.code == sf::Keyboard::A) a_pressed = true;
        if(event.key.code == sf::Keyboard::S) s_pressed = true;
        if(event.key.code == sf::Keyboard::D) d_pressed = true;
        if(event.key.code == sf::Keyboard::LShift) shift_pressed = true;
        if(event.key.code == sf::Keyboard::Space) space_pressed = true;
        if(event.key.code == sf::Keyboard::Up) up_pressed = true;
        if(event.key.code == sf::Keyboard::Left) left_pressed = true;
        if(event.key.code == sf::Keyboard::Down) down_pressed = true;
        if(event.key.code == sf::Keyboard::Right) right_pressed = true;
    } else if(event.type == sf::Event::KeyReleased) {
        if(event.key.code == sf::Keyboard::W) w_pressed = false;
        if(event.key.code == sf::Keyboard::A) a_pressed = false;
        if(event.key.code == sf::Keyboard::S) s_pressed = false;
        if(event.key.code == sf::Keyboard::D) d_pressed = false;
        if(event.key.code == sf::Keyboard::LShift) shift_pressed = false;
        if(event.key.code == sf::Keyboard::Space) space_pressed = false;
        if(event.key.code == sf::Keyboard::Up) up_pressed = false;
        if(event.key.code == sf::Keyboard::Left) left_pressed = false;
        if(event.key.code == sf::Keyboard::Down) down_pressed = false;
        if(event.key.code == sf::Keyboard::Right) right_pressed = false;
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

    double camera_move_y = ((up_pressed ? 1 : 0) + (down_pressed ? -1 : 0)) * 0.06;
    double camera_move_x = ((right_pressed ? 1 : 0) + (left_pressed ? -1 : 0)) * 0.06;

    position += {walk_x * heading_cos - walk_y * heading_sin, walk_x * heading_sin + walk_y * heading_cos, walk_z};

    double new_heading = fmod(heading + (double)camera_move_x, M_PI * 2);
    double new_pitch = controlled_camera->get_pitch() + (double)camera_move_y;
    if(new_pitch < -M_PI / 2) new_pitch = -M_PI / 2;
    if(new_pitch > M_PI / 2) new_pitch = M_PI / 2;

    controlled_camera->set_heading(new_heading);
    controlled_camera->set_pitch(new_pitch);
    controlled_camera->set_position(position);

}
