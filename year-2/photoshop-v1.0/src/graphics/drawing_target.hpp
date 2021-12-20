#pragma once

#include <SFML/Graphics.hpp>

class DrawingTarget {
protected:
    sf::RenderTarget* target = nullptr;
public:
    DrawingTarget() = default;

    inline sf::RenderTarget* get_target() { return target; }
};