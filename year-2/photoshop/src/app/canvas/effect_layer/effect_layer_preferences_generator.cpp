//
// Created by Артем on 11.11.2021.
//

#include "effect_layer_preferences_generator.hpp"
#include "../../photoshop_view.hpp"

void EffectLayerPreferencesGenerator::open_preferences(PhotoshopView* app) {
    if(!opened_window) {
        opened_window = new EffectLayerPreferencesWindow(layer);

        auto handler = new EventHandler<WindowCloseEvent>;
        *handler = [this, handler](WindowCloseEvent* event) {
            delete handler;
            opened_window = nullptr;
        };
        opened_window->get_close_event_emitter()->add_listener(handler);
        app->present_window(opened_window);
    } else {
        app->focus_window(opened_window);
    }
}

EffectLayerPreferencesGenerator::~EffectLayerPreferencesGenerator() { if(opened_window) opened_window->close(); }
