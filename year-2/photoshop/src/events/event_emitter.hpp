#pragma once

#include <vector>

template<typename EventType>
class EventHandler {
public:
    virtual void operator() (EventType* event) {}
};

template<typename EventType>
class EventEmitter {
    std::vector<EventHandler<EventType>*> handlers;
public:
    EventEmitter() {};

    void add_listener(EventHandler<EventType>* listener) {
        handlers.push_back(listener);
    }

    void remove_listener(EventHandler<EventType>* listener) {
        for(int i = 0; i < handlers.size(); i++) {
            if(handlers[i] == listener) {
                handlers.erase(handlers.begin() + i);
            }
        }
    };

    void emit(EventType* event) {
        for(int i = 0; i < handlers.size(); i++) {
            (*handlers[i])(event);
        }
    }
};