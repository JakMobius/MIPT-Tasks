
#include "brush_tool_preferences_generator.hpp"
#include "../../photoshop_view.hpp"

void BrushToolPreferencesGenerator::open_preferences(PhotoshopView* app) {
    if(!opened_window) {
        opened_window = new BrushToolPreferencesWindow(tool, {200, 200});

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
