#pragma once

#include <vector>
#include "./ui_animation.hpp"

class UIAnimationController {
    std::vector<UIAnimation*> animations {};
public:
    UIAnimationController() {

    }

    void add_animation(UIAnimation* animation);

    void tick(float dt) {
        for(int i = 0; i < animations.size(); i++) {
            if(!animations[i]->tick(dt)) {
                delete animations[i];
                animations.erase(animations.begin() + i);
                i--;
            }
        }
    }
};