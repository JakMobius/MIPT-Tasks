#pragma once

#include <vector>
#include "../drawing_context.hpp"

class Shape {
public:
    virtual void draw(DrawingContext* ctx) = 0;
};