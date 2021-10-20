#pragma once

#include "drawing_target.hpp"

class DrawingTargetWindow : public DrawingTarget {
public:
    DrawingTargetWindow(sf::RenderWindow* window) { target = window; }
};