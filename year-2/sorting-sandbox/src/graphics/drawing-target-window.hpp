#pragma once

#include "drawing-target.hpp"

class DrawingTargetWindow : public DrawingTarget {
public:
    DrawingTargetWindow(sf::RenderWindow* window) { target = window; }
};