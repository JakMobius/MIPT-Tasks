#pragma once

class Entity;

enum class EntityType {
    entity, wall, small_ball, super_ball
};

class ComponentBase;
#include "../utils/dynamic_array.hpp"

class Entity {
    dynamic_array<ComponentBase*> components;
    EntityType type;

public:
    Entity(EntityType type): components(), type(type) {};
    void add_component(ComponentBase* component) { components.push(component); }
    const dynamic_array<ComponentBase*>& get_components() { return components; }

    EntityType get_type() { return type; }
};