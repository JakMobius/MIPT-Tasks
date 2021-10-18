#pragma once

struct MouseEvent {
private:
    bool handled;

public:
    float x, y;

    MouseEvent(float x, float y): x(x), y(y) {};

    void mark_handled() { handled = true; }
    bool is_handled() { return handled; }
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

struct MouseMoveEvent : MouseEvent {
    float dx, dy;

    MouseMoveEvent(float x, float y, float dx, float dy): MouseEvent(x, y), dx(dx), dy(dy) {}
};
struct MouseOutEvent : MouseEvent {
    MouseOutEvent(float x, float y): MouseEvent(x, y) {}
};
struct MouseInEvent : MouseEvent {
    MouseInEvent(float x, float y): MouseEvent(x, y) {}
};