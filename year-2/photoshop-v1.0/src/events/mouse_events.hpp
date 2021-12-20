#pragma once

#include "event.hpp"

struct MouseEvent: public EventBase {
public:
    float x, y;

    MouseEvent(float x, float y): x(x), y(y) {};
};

struct MouseDownEvent : MouseEvent {
    MouseDownEvent(float x, float y): MouseEvent(x, y) {};
};

struct MouseUpEvent : MouseEvent {
    MouseUpEvent(float x, float y): MouseEvent(x, y) {};
};

struct MouseClickEvent : MouseEvent {
    MouseClickEvent(float x, float y): MouseEvent(x, y) {};
};

struct MouseScrollEvent : MouseEvent {
    float dx, dy;
    MouseScrollEvent(float x, float y, float dx, float dy): MouseEvent(x, y), dx(dx), dy(dy) {};
};

struct MouseMoveEvent : MouseEvent {
    MouseMoveEvent(float x, float y): MouseEvent(x, y) {}
};
struct MouseOutEvent : MouseEvent {
    MouseOutEvent(float x, float y): MouseEvent(x, y) {}
};
struct MouseInEvent : MouseEvent {
    MouseInEvent(float x, float y): MouseEvent(x, y) {}
};