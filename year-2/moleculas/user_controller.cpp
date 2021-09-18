
#include "user_controller.hpp"

void UserController::handle_event(const sf::Event &event)  {
    if (event.type == sf::Event::Closed) window->close();
}

void UserController::tick() {

}
