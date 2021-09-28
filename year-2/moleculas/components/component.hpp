#pragma once

class ComponentBase;
class ComponentDelegate;

#include "entity.hpp"

class ComponentBase {
protected:
    Entity* entity;

public:
    explicit ComponentBase(Entity* entity): entity(entity) {}

    Entity* get_entity() { return entity; }
};

class ComponentDelegate {
public:
    virtual Entity* get_this() = 0;
};

template <typename T = ComponentDelegate>
class Component : public ComponentBase {
    T* delegate;
public:

    explicit Component(T* delegate): ComponentBase(delegate->get_this()), delegate(delegate) {};

    T* get_delegate() { return delegate; }
};