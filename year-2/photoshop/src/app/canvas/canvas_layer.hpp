#pragma once

class CanvasLayer;
class Canvas;
struct LayerUpdateEvent {
    CanvasLayer* layer;
};

#include "../../graphics/drawing_target_texture.hpp"
#include "../../graphics/drawing_context.hpp"
#include "../../events/event_emitter.hpp"
#include "../preferences_generator.hpp"
#include "../../ui/styles/fill_style/fill_style_texture.hpp"
#include "../adjustable.hpp"

class CanvasLayer : public Adjustable {
protected:
    EventEmitter<LayerUpdateEvent> layer_update_event_emitter {};
    DrawingTargetTexture* texture = nullptr;
    DrawingTargetTexture* preview_texture = nullptr;
    UIFillStyleTexture draw_style {};
    Vec2i size;
    bool needs_redraw = false;
    bool needs_clear = true;

    virtual void create_preferences_generator() {};

public:
    explicit CanvasLayer(Vec2i size);
    virtual ~CanvasLayer();

    virtual void clear_texture();
    virtual void draw(Canvas* canvas, DrawingContext* ctx);

    PreferencesGenerator* get_preferences_generator() {
        if(!preferences_generator) create_preferences_generator();
        return preferences_generator;
    }

    void set_needs_redraw();
    bool get_needs_redraw() const { return needs_redraw; }
    EventEmitter<LayerUpdateEvent>* get_layer_update_event_emitter() { return &layer_update_event_emitter; }

    DrawingTargetTexture* get_texture() { return texture; }
    DrawingTargetTexture* get_preview_texture() { return preview_texture; }

    Vec2i get_size() const { return size; }
};