#pragma once

#include <vector>
#include "../drawing-context.hpp"

class Shape {
public:
    virtual void draw(DrawingContext* ctx) = 0;
};