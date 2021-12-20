#pragma once

#include <vector>
#include "../../../utils/vec2.hpp"

class CatmullRomCurve {
    std::vector<Vec2f> points {};
    float alpha = 0.5;

    static float get_t(float t, float alpha, const Vec2f &p0, const Vec2f &p1);
    static Vec2f evaluate(const Vec2f &p0, const Vec2f &p1, const Vec2f &p2, const Vec2f &p3, float t, float alpha);

public:
    CatmullRomCurve();
    CatmullRomCurve(const CatmullRomCurve& copy) = default;
    CatmullRomCurve& operator=(const CatmullRomCurve& copy_assignment) = default;

    int get_steps() const { return points.size() - 3; }

    Vec2f get_point(int step, float t) const {
        return evaluate(points[step], points[step + 1], points[step + 2], points[step + 3], t, alpha);
    }

    int find_next_point_by_x(float x);

    int find_nearest_point_by_x(float x);

    std::vector<Vec2f>& get_points() { return points; }
};