//
// Created by Артем on 09.11.2021.
//

#include "layer_inspector_window.hpp"
#include "../canvas/effect_layer/canvas_effect_layer.hpp"

UIFillStyleColor LayerListItemView::selected_background({0.1, 0.1, 0.2, 1.0});

void LayerInspectorWindow::update_list() {
    if(!active_canvas) {
        update_list_size(0);
        return;
    }

    int list_size = (int)active_canvas->get_layers().size();

    update_list_size(list_size);

    for(int i = 0; i < list_size; i++) {
        layer_list_item_views[i]->preview_layer(active_canvas->get_layers()[i]);
    }
}

void LayerInspectorWindow::update_list_size(int n) {
    while(layer_list_item_views.size() > n) {
        auto* item = layer_list_item_views[layer_list_item_views.size() - 1];
        layer_list_item_views.pop_back();
        int index = layers_scroll_view->get_content_view()->get_child_index(item);
        if(index >= 0) layers_scroll_view->get_content_view()->remove_child(index);
        delete item;
    }

    while(layer_list_item_views.size() < n) {
        auto* item = new LayerListItemView(this);
        item->set_position({0, 75.f * layer_list_item_views.size()});
        layer_list_item_views.push_back(item);
        layers_scroll_view->get_content_view()->append_child(item);
    }
}

LayerInspectorWindow::LayerInspectorWindow(PhotoshopView* app, ToolManager* manager, const Vec2f& position) : PhotoshopWindow(app, position, {250, 500}), manager(manager) {
    set_title("Layer inspector");

    layers_scroll_view = new UIScrollView({}, {250, 450});
    get_content_view()->append_child(layers_scroll_view);
    manager->get_canvas_change_event_emitter()->add_listener(&canvas_change_event_listener);

    setup_bottom_bar();

    get_content_view()->append_child(bottom_button_container);
    on_canvas_changed();
}

void LayerInspectorWindow::on_canvas_changed() {
    if(active_canvas) active_canvas->get_update_event_emitter()->remove_listener(&canvas_update_event_listener);
    active_canvas = manager->get_active_canvas();
    if(active_canvas) active_canvas->get_update_event_emitter()->add_listener(&canvas_update_event_listener);

    update_list();

    if(active_canvas) {

        auto active_layer = active_canvas->get_active_layer();

        effect_layer_add_button->set_enabled(true);
        layer_add_button->set_enabled(true);

        if(active_layer) {
            layer_remove_button->set_enabled(true);
            layer_preferences_button->set_enabled(active_layer->get_preferences_generator() != nullptr);
        } else {
            layer_remove_button->set_enabled(false);
            layer_preferences_button->set_enabled(false);
        }
    } else {
        effect_layer_add_button->set_enabled(false);
        layer_add_button->set_enabled(false);
        layer_remove_button->set_enabled(false);
        layer_preferences_button->set_enabled(false);
    }
}

LayerInspectorWindow::~LayerInspectorWindow() {
    if(active_canvas) {
        active_canvas->get_update_event_emitter()->remove_listener(&canvas_update_event_listener);
    }
    manager->get_canvas_change_event_emitter()->remove_listener(&canvas_change_event_listener);
}

void LayerInspectorWindow::setup_bottom_bar() {
    bottom_button_container = new UIStackView(UIAxis::x, {0, 450});
    bottom_button_container->set_fitting({250, 50});
    bottom_button_container->set_lateral_alignment(UIStackViewLateralAlignment::center);
    bottom_button_container->set_primary_alignment(UIStackViewPrimaryAlignment::space_between);
    bottom_button_container->set_insets({ 0, 10 });
    bottom_button_container->set_item_spacing(10);

    auto* left_button_container = new UIStackView(UIAxis::x);
    left_button_container->set_item_spacing(10);

    auto* right_button_container = new UIStackView(UIAxis::x);
    right_button_container->set_item_spacing(10);

    bottom_button_container->append_child(left_button_container);
    bottom_button_container->append_child(right_button_container);

    effect_layer_add_button = new UIButton({}, {40, 30});
    layer_add_button = new UIButton({}, {40, 30});
    layer_remove_button = new UIButton({}, {40, 30});
    layer_preferences_button = new UIButton({}, {40, 30});

    effect_layer_add_button->set_title("+E");
    layer_add_button->set_title("+");
    layer_remove_button->set_title("-");
    layer_preferences_button->set_title("P");

    right_button_container->append_child(effect_layer_add_button);
    right_button_container->append_child(layer_add_button);
    right_button_container->append_child(layer_remove_button);
    left_button_container->append_child(layer_preferences_button);

    layer_add_button->set_callback([this]() {
        active_canvas->add_layer(new CanvasLayer(active_canvas->get_size()));
    });

    effect_layer_add_button->set_callback([this]() {
        active_canvas->add_layer(new CanvasEffectLayer(active_canvas->get_size()));
    });

    layer_remove_button->set_callback([this]() {
        auto layer = active_canvas->get_active_layer();
        active_canvas->remove_layer(layer);
        delete layer;
    });

    layer_preferences_button->set_callback([this]() {
        auto generator = active_canvas->get_active_layer()->get_preferences_generator();
        if(generator) generator->open_preferences(app);
    });
}

LayerListItemView::LayerListItemView(LayerInspectorWindow* window) : UIView({}, {250, 75}), window(window) {
    auto text = new UIText({}, {100, 75}, "Layer");
    text->get_text_drawer()->set_v_alignment(VTextAlignmentCenter);
    text->get_text_drawer()->set_h_alignment(HTextAlignmentCenter);
    append_child(text);

    auto border_view = new UIView({0, 74}, {250, 1});
    border_view->set_fill_style(&UIViewWhiteBackground);
    append_child(border_view);

    layer_present_view_container = new UIView({120, 10}, {100, 55});
    layer_present_view_background = new UIView();
    layer_present_view_background->set_fill_style(&imaged_background);
    layer_present_view = new UIView();
    layer_present_view->set_fill_style(layer_preview_view_style);
    layer_present_view_container->append_child(layer_present_view_background);
    layer_present_view_background->append_child(layer_present_view);
    append_child(layer_present_view_container);
}

void LayerListItemView::preview_layer(CanvasLayer* layer) {
    if(previewed_layer == layer) {
        update_background();
        return;
    }

    if(previewed_layer) {
        previewed_layer->get_layer_update_event_emitter()->remove_listener(&layer_update_event_listener);
    }

    previewed_layer = layer;

    update_texture();

    set_needs_redraw();
}

void LayerListItemView::update_texture() {
    if(!previewed_layer) {
        layer_preview_view_style->set_texture(nullptr);
        return;
    }

    previewed_layer->get_layer_update_event_emitter()->add_listener(&layer_update_event_listener);

    Vec2f texture_size = (Vec2f)previewed_layer->get_texture()->get_size();

    float scale = 1;
    for(int i = 0; i < 2; i++) {
        float axle_scale = texture_size[i] / (float)layer_present_view_container->get_size()[i];
        if(axle_scale > scale) scale = axle_scale;
    }

    Vec2f icon_size = texture_size / scale;

    layer_present_view_background->set_size(icon_size);
    layer_present_view_background->set_position(layer_present_view_container->get_size() * 0.5 -= icon_size * 0.5);
    layer_present_view->set_size(icon_size);
    layer_preview_view_style->set_scale({scale, scale});
    layer_preview_view_style->set_texture(previewed_layer->get_texture());

    update_background();
}

void LayerListItemView::update_background() {
    bool current_is_active = window->get_manager()->get_active_canvas()->get_active_layer() == previewed_layer;
    if(current_is_active == is_layer_active) return;
    is_layer_active = current_is_active;
    if(is_layer_active) {
        set_fill_style(&selected_background);
    } else {
        set_fill_style(nullptr);
    }
}

void LayerListItemView::on_mouse_click(MouseClickEvent* event) {
    if(is_layer_active) return;
    window->get_manager()->get_active_canvas()->set_active_layer(previewed_layer);
}

LayerListItemView::~LayerListItemView() {
    if(previewed_layer) {
        previewed_layer->get_layer_update_event_emitter()->remove_listener(&layer_update_event_listener);
    }
    delete layer_preview_view_style;
}
