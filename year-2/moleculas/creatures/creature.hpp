#pragma once

#include "../drawing_context.hpp"
#include "../fixtures/fixture.hpp"

class Creature {
protected:
    Fixture* fixture;
    Creature(): fixture(nullptr) {};

public:
    virtual ~Creature() {};

    Creature(const Creature& other) = delete;
    Creature(Creature&& other) = delete;
    Creature& operator=(const Creature* other) = delete;
    Creature& operator=(Creature&& other) = delete;

    virtual void draw(DrawingContext* ctx) = 0;

    Fixture* get_fixture() { return fixture; }
};