#pragma once

class GameObject;

#include "../../visual-engine/drawing_context.hpp"
#include "../../physics-engine/bodies/body.hpp"
#include "../../components/component.hpp"
#include "../../components/entity.hpp"
#include "../../visual-engine/visual_component.hpp"
#include "../scene.hpp"

class GameObject : public Entity, public Physics::RigidBodyDelegate, public VisualComponentDelegate {
protected:
    Physics::RigidBody* body;
    GameScene* scene;
    bool dead;

    GameObject(EntityType type) : Entity(type), body(nullptr), dead(false), scene(nullptr) {};

public:
    virtual ~GameObject() { delete body; };
    GameObject(const GameObject &other) = delete;
    GameObject(GameObject &&other) = delete;
    GameObject &operator=(const GameObject* other) = delete;
    GameObject &operator=(GameObject &&other) = delete;
    virtual void draw(DrawingContext* ctx) = 0;

    Physics::RigidBody* get_physical_body() { return body; }

    void kill() { dead = true; }
    bool is_dead() const { return dead; }

    void set_scene(GameScene* scene) { this->scene = scene; }

    Entity* get_this() override { return this; }
};