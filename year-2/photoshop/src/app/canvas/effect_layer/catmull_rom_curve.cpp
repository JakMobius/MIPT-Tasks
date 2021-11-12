//
// Created by Артем on 11.11.2021.
//

#include "catmull_rom_curve.hpp"

Vec2f CatmullRomCurve::evaluate(const Vec2f &p0, const Vec2f &p1, const Vec2f &p2, const Vec2f &p3, float t, float alpha) {
    float t0 = 0.0f;
    float t1 = get_t( t0, alpha, p0, p1 );
    float t2 = get_t( t1, alpha, p1, p2 );
    float t3 = get_t( t2, alpha, p2, p3 );

    float ta = t1 * (1 - t) + t2 * t;

    Vec2f A1 = p0 * ((t1 - ta) / (t1 - t0)) + p1 * ((ta - t0) / (t1 - t0));
    Vec2f A2 = p1 * ((t2 - ta) / (t2 - t1)) + p2 * ((ta - t1) / (t2 - t1));
    Vec2f A3 = p2 * ((t3 - ta) / (t3 - t2)) + p3 * ((ta - t2) / (t3 - t2));
    Vec2f B1 = A1 * ((t2 - ta) / (t2 - t0)) + A2 * ((ta - t0) / (t2 - t0));
    Vec2f B2 = A2 * ((t3 - ta) / (t3 - t1)) + A3 * ((ta - t1) / (t3 - t1));
    Vec2f C  = B1 * ((t2 - ta) / (t2 - t1)) + B2 * ((ta - t1) / (t2 - t1));

    return C;
}

float CatmullRomCurve::get_t(float t, float alpha, const Vec2f &p0, const Vec2f &p1) {
    auto d = p1 - p0;
    float a = d.dot(d);
    float b = pow(a, alpha * .5f);
    return b + t;
}

int CatmullRomCurve::find_next_point_by_x(float x) {

    int min_index = 0;
    int max_index = points.size() - 1;

    while(min_index != max_index) {
        int mid_index = (min_index + max_index) / 2;
        float mid_x = points[mid_index][0];

        if(mid_index == min_index) {
            if(mid_x < x) return max_index;
            return min_index;
        }

        if(mid_x > x) max_index = mid_index;
        else if(mid_x < x) min_index = mid_index;
        else return mid_index;
    }

    return min_index;
}

int CatmullRomCurve::find_nearest_point_by_x(float x) {
    int nearest_index = find_next_point_by_x(x);
    if(nearest_index <= 0) return nearest_index;

    float distance_to_next = points[nearest_index][0] - x;
    float distance_to_prev = x - points[nearest_index - 1][0];

    if(distance_to_prev < distance_to_next) nearest_index--;

    return nearest_index;
}

CatmullRomCurve::CatmullRomCurve() {
    points.emplace_back(-1, -1);
    points.emplace_back(0, 0);
    points.emplace_back(1, 1);
    points.emplace_back(2, 2);
}
