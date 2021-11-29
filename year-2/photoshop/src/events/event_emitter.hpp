#pragma once

#include <vector>
#include <functional>

template <typename EventType>
using EventHandler = std::function<void(EventType*)>;

template<typename EventType>
class EventEmitter {
    std::vector<EventHandler<EventType>*> handlers;
    std::vector<EventHandler<EventType>*> ignored_handlers;
    bool is_emitting = false;

    bool is_ignored(EventHandler<EventType>* listener);

public:
    EventEmitter() {};

    void add_listener(EventHandler<EventType>* listener);

    const  std::vector<EventHandler<EventType>*>& get_listeners() { return handlers; }

    void remove_listener(EventHandler<EventType>* listener);;

    void emit(EventType* event);
};

template<typename EventType>
void EventEmitter<EventType>::emit(EventType* event) {
    is_emitting = true;
    for(int i = 0; i < handlers.size(); i++) {
        if(!is_ignored(handlers[i])) (*handlers[i])(event);
    }
    is_emitting = false;
    for(int i = 0; i < ignored_handlers.size(); i++) {
        remove_listener(ignored_handlers[i]);
    }
    ignored_handlers.clear();
}

template<typename EventType>
void EventEmitter<EventType>::remove_listener(EventHandler<EventType>* listener) {
    if(is_emitting) {
        ignored_handlers.push_back(listener);
    } else {
        for(int i = 0; i < handlers.size(); i++) {
            if(handlers[i] == listener) {
                handlers.erase(handlers.begin() + i);
                break;
            }
        }
    }
}

template<typename EventType>
void EventEmitter<EventType>::add_listener(EventHandler<EventType>* listener) {
    handlers.push_back(listener);
}

template<typename EventType>
bool EventEmitter<EventType>::is_ignored(EventHandler<EventType>* listener) {
    for(int i = 0; i < ignored_handlers.size(); i++) {
        if(ignored_handlers[i] == listener) return true;
    }
    return false;
}
