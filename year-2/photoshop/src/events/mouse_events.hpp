#pragma once

struct MouseEvent {
private:
    bool handled = false;
    bool propagate = true;

public:
    float x, y;

    MouseEvent(float x, float y): x(x), y(y) {};

    void stop_propagation() { propagate = false; }
    void mark_handled() { handled = true; }
    bool is_handled() const { return handled; }
    bool should_propagate() const { return propagate; }
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