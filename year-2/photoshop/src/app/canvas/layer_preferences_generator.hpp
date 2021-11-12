#pragma once

class LayerPreferencesGenerator;

#include "canvas_layer.hpp"
#include "../photoshop_view.hpp"

class LayerPreferencesGenerator {
public:
    explicit LayerPreferencesGenerator() {}
    virtual ~LayerPreferencesGenerator() {};

    virtual void open_preferences(PhotoshopView* app) = 0;
};