//
// Created by Артем on 11.11.2021.
//

#include "effect_layer_preferences_window.hpp"


EffectLayerPreferencesWindow::EffectLayerPreferencesWindow(CanvasEffectLayer* layer) : UIWindow({}, {360, 300}), layer(layer) {
    set_title("Effect preferences");

    const char* titles[] = {"R", "G", "B"};

    for(int i = 0; i < 3; i++) {
        Vec4f curve_color { 0, 0, 0, 1 };
        curve_color.set(i, 1.0);

        auto* view = new CatmullRomColorCurveView({10, 10}, {280, 280});
        view->set_curve(layer->get_curve(0));
        view->set_curve_color(curve_color);
        view->set_callback([this, i]() {
            this->layer->set_curve(i, catmull_rom_views[i]->get_curve());
        });
        catmull_rom_views[i] = view;
        get_content_view()->append_child(view);

        auto button = new UIButton({310, 10 + 50.f * i}, {40, 40});
        button->set_title(titles[i]);

        button->set_callback([this, i]() { handle_button_click(i); });

        catmull_rom_buttons[i] = button;
        get_content_view()->append_child(button);
    }
}

void EffectLayerPreferencesWindow::handle_button_click(int i) {
    for(int channel_index = 0; channel_index < 3; channel_index++) {
        bool enabled = i == channel_index;

        Vec4f curve_color { 0, 0, 0, 1 };
        curve_color.set(channel_index, enabled ? 1.0 : 0.5);

        Vec4f point_color { 0.5, 0.5, 0.5, 1 };
        if(enabled) point_color = { 1, 1, 1, 1 };

        catmull_rom_buttons[channel_index]->set_selected(enabled);
        catmull_rom_views[channel_index]->set_interactions_enabled(enabled);
        catmull_rom_views[channel_index]->set_curve_color(curve_color);
        catmull_rom_views[channel_index]->set_point_color(point_color);
    }
    int index = get_content_view()->get_child_index(catmull_rom_views[i]);
    if(index >= 0) {
        get_content_view()->remove_child(index);
        get_content_view()->append_child(catmull_rom_views[i]);
    }
}
