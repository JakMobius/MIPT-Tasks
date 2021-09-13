//
// Created by Артем on 13.09.2021.
//

#include "point_test_iterator.hpp"

UIView* PointTestIterator::next(Vec2d* internal_point) {
    for(int i = current_child_index; i < view->get_children().size; i++) {
        UIView* child = view->get_children()[i];
        Vec2d absolute_point = point - child->get_position();
        absolute_point.transform(child->get_inv_transform());
        if(absolute_point.x < 0 || absolute_point.x >= child->get_size().x) continue;
        if(absolute_point.y < 0 || absolute_point.y >= child->get_size().y) continue;
        current_child_index++;
        *internal_point = absolute_point;
        return child;
    }
    return nullptr;
}
