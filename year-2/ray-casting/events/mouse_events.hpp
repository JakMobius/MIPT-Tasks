#pragma once

struct MouseEvent {
    double x, y;

    MouseEvent(double x, double y): x(x), y(y) {};
};

struct MouseDownEvent : MouseEvent {
    MouseDownEvent(double x, double y): MouseEvent(x, y) {};
};

struct MouseUpEvent : MouseEvent {
    MouseUpEvent(double x, double y): MouseEvent(x, y) {};
};

struct MouseMoveEvent : MouseEvent {
    double dx, dy;

    MouseMoveEvent(double x, double y, double dx, double dy): MouseEvent(x, y), dx(dx), dy(dy) {}
};
struct MouseOutEvent : MouseEvent {
    MouseOutEvent(double x, double y): MouseEvent(x, y) {}
};
struct MouseInEvent : MouseEvent {
    MouseInEvent(double x, double y): MouseEvent(x, y) {}
};