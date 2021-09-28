#pragma once

class GameScene;

#include "../visual-engine/drawing_context.hpp"
#include "../physics-engine/world/world.hpp"
#include "creatures/game_object.hpp"
#include "../visual-engine/ui/ui_view.hpp"
#include "interface_view.hpp"

class GameScene : public UIView {
    Physics::World* physics_world;
    dynamic_array<GameObject*> creatures;
    InterfaceView interface;
    bool reactions_on;

public:
    GameScene(const Vec2d& position = {0, 0}, const Vec2d& size = {0, 0});
    ~GameScene();
    GameScene(const GameScene &other) = delete;
    GameScene(GameScene &&other) = delete;
    GameScene &operator=(const GameScene* other) = delete;
    GameScene &operator=(GameScene &&other) = delete;

    void draw(DrawingContext* ctx) override;

    void add_creature(GameObject* creature);

    void init_scene();

    void tick(double dt);

    const Physics::World* get_physics_world();

    bool are_reactions_on() { return reactions_on; }
};