//
// Created by Артем on 26.09.2021.
//

#include "scene.hpp"
#include "creatures/plane.hpp"
#include "creatures/circle.hpp"
#include "creatures/super_circle.hpp"

GameScene::GameScene(const Vec2d& position, const Vec2d& size): UIView(position, size), interface(), reactions_on(true) {
    physics_world = new Physics::World();

    append_child(&interface);
    interface.set_position(size - Vec2d(400, 120));
    set_background({0, 0, 0, 0});

    interface.get_reaction_toggle_button()->set_callback([this]() -> void {
        this->reactions_on = !this->reactions_on;
        interface.update_reactions_button(this->reactions_on);
    });

    interface.update_reactions_button(this->reactions_on);
    interface.get_slider()->set_fraction(0);

}

void GameScene::draw(DrawingContext* ctx) {
    UIView::draw(ctx);
    for(int i = 0; i < creatures.size; i++) {
        creatures[i]->draw(ctx);
    }
}

void GameScene::tick(double dt) {
    physics_world->tick(dt * interface.get_slider()->get_fraction(), 50);

    int new_i = 0;

    for(int i = 0; i < creatures.size; i++) {
        auto creature = creatures[i];
        if(creature->is_dead()) {
            physics_world->delete_body(creature->get_physical_body());
            delete creature;
        } else {
            if(i != new_i) creatures[new_i] = creatures[i];
            new_i++;
        }
    }

    creatures.size = new_i;
}

void GameScene::init_scene() {
    add_creature(new Plane({1000, 10}, Physics::PlaneOrientation::vertical));
    add_creature(new Plane({10, 10},   Physics::PlaneOrientation::vertical));
    add_creature(new Plane({10, 10},   Physics::PlaneOrientation::horizontal));
    add_creature(new Plane({10, 1000}, Physics::PlaneOrientation::horizontal));

    for(int i = 0; i < 15; i++) {
        for(int j = 0; j < 15; j++) {
            auto new_circle = new Circle({100 + 40 * (double) i, 100 + 40 * (double) j}, 10);
            add_creature(new_circle);
            new_circle->get_physical_body()->set_velocity(Vec2d {sin(j) + cos(i), cos(j) + sin(i)} *= 200);
        }
    }
}

const Physics::World* GameScene::get_physics_world() {
    return physics_world;
}

GameScene::~GameScene() {
    for(int i = 0; i < creatures.size; i++) {
        delete creatures[i];
    }
    delete physics_world;
}

void GameScene::add_creature(GameObject* creature) {
    creatures.push(creature);
    creature->set_scene(this);
    physics_world->add_body(creature->get_physical_body());
}
