//
// Created by Артем on 06.09.2021.
//

#include "awesome_functions.hpp"
#include <cmath>

float parabola(float x) {
    return x * x * sin(x * 1);
}

float taylor_sine(float x) {
    int terms = 15;
    double result = x;
    double numerator = x;
    double denominator = 1;

    for(int i = 1; i < terms; i++) {
        numerator *= x * x;
        denominator *= i * (i * 4 + 2);
        double term = numerator / denominator;
        if((i & 1) == 1) term = -term;
        result += term;
    }

    return (float)result;
}

float float_hash(float a) {
    return (float)(*((int*)(&a)) % 100) / 100;
}