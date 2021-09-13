#pragma once

class PointTestIterator;

#include "ui_view.hpp"
#include "../utils/vec2.hpp"

class PointTestIterator {
    const UIView* view;
    Vec2d point;
    int current_child_index = 0;

public:
    PointTestIterator(const UIView* view, const Vec2d& point) {
        this->view = view;
        this->point = point;
    }

    UIView* next(Vec2d* internal_point);
};