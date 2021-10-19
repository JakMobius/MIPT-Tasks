#pragma once

#include "../graphics/drawing_context.hpp"

class UIDrawingContext : public DrawingContext {
    bool is_context_active = true;
public:
    explicit UIDrawingContext(sf::RenderWindow* window): DrawingContext(window) {}
    void set_context_active(bool p_is_context_active) { is_context_active = p_is_context_active; };
    bool get_context_active() const { return is_context_active; }
};