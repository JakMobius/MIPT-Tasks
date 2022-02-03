#pragma once

struct KeyModifiers {
    bool shift = false;
    bool super = false;
    bool alt = false;
    bool control = false;
};

class EventBase {
    bool handled = false;
    bool propagate = true;
public:

    void stop_propagation() { propagate = false; }
    void mark_handled() { handled = true; }
    bool is_handled() const { return handled; }
    bool should_propagate() const { return propagate; }
};