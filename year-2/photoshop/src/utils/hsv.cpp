//
// Created by Артем on 23.10.2021.
//

#include "hsv.hpp"
#include "vec2.hpp"


// hue: 0 - 360
// saturation: 0 - 1
// value: 0 - 1
Vec3f rgb_from_hsv(float hue, float saturation, float value) {
    if(hue > 360 || hue < 0 || saturation > 1 || saturation < 0 || value > 1 || value < 0) {
        return {0, 0, 0};
    }

    float coef1 = saturation * value;
    float coef2 = coef1 * (1.f - abs(fmod(hue / 60.f, 2.f) - 1.f));
    float m = value - coef1;

    Vec3f result;
    if(hue >= 0 && hue < 60) result = {coef1, coef2, 0};
    else if(hue >= 60 && hue < 120) result = {coef2, coef1, 0};
    else if(hue >= 120 && hue < 180) result = {0, coef1, coef2};
    else if(hue >= 180 && hue < 240) result = {0, coef2, coef1};
    else if(hue >= 240 && hue < 300) result = {coef2, 0, coef1};
    else result = {coef1, 0, coef2};

    result += {m, m, m};
    return result;
}

static float min3(float a, float b, float c) {
    if(a < b && a < c) return a;
    if(b < c && b < a) return b;
    return c;
}

static float max3(float a, float b, float c) {
    if(a > b && a > c) return a;
    if(b > c && b > a) return b;
    return c;
}

Vec3f hsv_from_rgb(float red, float green, float blue) {
    Vec3f out {};

    float min = min3( red, green, blue );
    float max = max3( red, green, blue );

    out.set_z(max);
    float delta = max - min;
    if(delta < FLOAT_EPS) {
        out.set_x(0);
        out.set_y(0);
        return out;
    }
    if(max > 0) {
        out.set_y(delta / max);
    } else {
        out.set_y(0);
        out.set_y(0);
        return out;
    }

    if(red >= max) out.set_x((green - blue) / delta);
    else if(green >= max) out.set_x(2.f + (blue - red) / delta);
    else out.set_x(4.f + (red - green) / delta);

    out.set_x(out[0] * 60);

    if(out[0] < 0) out.set_x(out[0] + 360);

    return out;
}