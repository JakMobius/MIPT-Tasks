#pragma once

class UIAnimation {
public:
    virtual ~UIAnimation() {};
    virtual bool tick(float dt) = 0;
};