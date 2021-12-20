//
// Created by Артем on 11.11.2021.
//

#include "catmull_rom_view.hpp"


CatmullRomColorCurveView::CatmullRomColorCurveView(const Vec2f &position, const Vec2f &size) : UIView(position, size) {
    set_masks_to_bounds(true);
}

void CatmullRomColorCurveView::draw(DrawingContext* ctx) {
    UIView::draw(ctx);

    ctx->set_fill_style(&curve_color);
    line_shape.clear_shape();
    line_shape.set_thickness(3);

    for(int step = 0; step < curve.get_steps(); step++) {
        for(int i = 0; i < steps; i++) {
            Vec2f position = curve.get_point(step, float(i) / float(steps));

            if(position[0] < curve.get_points()[step + 1][0]) position.set_x(curve.get_points()[step + 1][0]);
            if(position[0] > curve.get_points()[step + 2][0]) position.set_x(curve.get_points()[step + 2][0]);
            if(position[1] < 0) position.set_y(0);
            if(position[1] > 1) position.set_y(1);

            line_shape.add_point(mouse_position_from_graph(position));
        }
    }

    line_shape.add_point(mouse_position_from_graph(curve.get_points()[curve.get_points().size() - 2]));
    line_shape.draw(ctx);

    for(int i = 1; i < curve.get_points().size() - 1; i++) {
        if(i == hovered_point_index || i == captured_point_index) {
            ctx->set_fill_style(&hovered_point_color);
            ctx->fill_circle(mouse_position_from_graph(curve.get_points()[i]), 4);
        } else {
            ctx->set_fill_style(&point_color);
            ctx->fill_circle(mouse_position_from_graph(curve.get_points()[i]), 3);
        }
    }
}

void CatmullRomColorCurveView::on_mouse_move(MouseMoveEvent* event) {
    UIView::on_mouse_move(event);
    if(captured_point_index >= 0) {
        handle_mouse_position(event->x, event->y);
        points_updated();
    } else {
        update_hovered_point(event->x, event->y);
    }
}


void CatmullRomColorCurveView::on_mouse_down(MouseDownEvent* event) {
    UIView::on_mouse_down(event);

    Vec2f position = graph_position_from_mouse(event->x, event->y);
    int index = curve.find_nearest_point_by_x(position[0]);

    if(index < 0 || distance_to_point({position[0], position[1]}, index) * graph_size[0] > mouse_capture_distance) {
        if(index < 2) index = 2;
        curve.get_points().insert(curve.get_points().begin() + index, {0, 0});
    }

    captured_point_index = index;
    handle_mouse_position(event->x, event->y);

    points_updated();
}

void CatmullRomColorCurveView::on_mouse_up(MouseUpEvent* event) {
    UIView::on_mouse_up(event);

    if(captured_point_index == -1) return;

    Vec2f position = graph_position_from_mouse(event->x, event->y);

    bool should_merge = false;

    if(distance_to_point(position, captured_point_index - 1) < min_point_distance) should_merge = true;
    if(distance_to_point(position, captured_point_index + 1) < min_point_distance) should_merge = true;

    if(should_merge){
        curve.get_points().erase(curve.get_points().begin() + captured_point_index);
        points_updated();
    }

    captured_point_index = -1;
}

void CatmullRomColorCurveView::layout() {
    UIView::layout();
    graph_size = size;
    graph_size -= graph_position * 2;
}

void CatmullRomColorCurveView::handle_mouse_position(float x, float y) {

    Vec2f current_point = graph_position_from_mouse(x, y);

    if(current_point[1] > 1) current_point.set_y(1);
    else if(current_point[1] < 0) current_point.set_y(0);

    if(captured_point_index == curve.get_points().size() - 2)  current_point.set_x(1);
    if(captured_point_index == 1) current_point.set_x(0);

    if(current_point[0] < 0 || current_point[0] > 1) {
        curve.get_points().erase(curve.get_points().begin() + captured_point_index);
        captured_point_index = -1;
    } else if(current_point[0] > curve.get_points()[captured_point_index + 1][0]) {

        Vec2f b = curve.get_points()[captured_point_index + 1];

        curve.get_points()[captured_point_index] = b;
        curve.get_points()[captured_point_index + 1] = current_point;
        captured_point_index++;
        hovered_point_index = captured_point_index;
    } else if(current_point[0] < curve.get_points()[captured_point_index - 1][0]) {
        Vec2f b = curve.get_points()[captured_point_index - 1];

        curve.get_points()[captured_point_index] = b;
        curve.get_points()[captured_point_index - 1] = current_point;
        captured_point_index--;
        hovered_point_index = captured_point_index;
    } else {
        curve.get_points()[captured_point_index] = current_point;
    }
}

void CatmullRomColorCurveView::points_updated() {

    // Updating meta points

    Vec2f& last_meta_point = curve.get_points()[curve.get_points().size() - 1];
    Vec2f& first_meta_point = curve.get_points()[0];

    Vec2f& last_point = curve.get_points()[curve.get_points().size() - 2];
    Vec2f& first_point = curve.get_points()[1];

    last_meta_point.set_y(last_point[1] * 2 - first_point[1]);
    first_meta_point.set_y(first_point[1] * 2 - last_point[1]);

    if(callback) callback();

    set_needs_redraw();
}

float CatmullRomColorCurveView::distance_to_point(const Vec2f& pos, int index) {
    return (curve.get_points()[index] - pos).len();
}

Vec2f CatmullRomColorCurveView::graph_position_from_mouse(float x, float y) {
    return {(x - graph_position[0]) / graph_size[0], 1 - ((y - graph_position[0]) / graph_size[1])};
}

Vec2f CatmullRomColorCurveView::mouse_position_from_graph(const Vec2f& position) {
    Vec2f result(position);
    result.set_y(1 - position[1]);
    result *= graph_size;
    result += graph_position;
    return result;
}

void CatmullRomColorCurveView::update_hovered_point(float x, float y) {
    Vec2f position = graph_position_from_mouse(x, y);
    int index = curve.find_nearest_point_by_x(position[0]);

    if(index >= 0 && distance_to_point({position[0], position[1]}, index) * graph_size[0] < mouse_capture_distance) {
        if(hovered_point_index != index) set_needs_redraw();
        hovered_point_index = index;
    } else {
        if(hovered_point_index != -1) set_needs_redraw();
        hovered_point_index = -1;
    }
}

void CatmullRomColorCurveView::on_mouse_out(MouseOutEvent* event) {
    if(hovered_point_index != -1) set_needs_redraw();
    hovered_point_index = -1;
}

void CatmullRomColorCurveView::set_curve(const CatmullRomCurve &p_curve) {
    captured_point_index = -1;
    hovered_point_index = -1;
    curve = p_curve;
}
