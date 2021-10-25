//
// Created by Артем on 23.10.2021.
//

#include "hue_saturation_select_view.hpp"

void HueSaturationSelectView::create_texture(

        ) {
    auto size = (Vec2i)get_size();
    int pixels = size[0] * size[1];
    auto* rgba_array = new unsigned char[pixels * 4];
    for(int y = 0, texel = 0; y < size[1]; y++) {
        for(int x = 0; x < size[0]; x++) {
            float hue = (float(x) / float(size[0])) * 360;
            float saturation = 1 - float(y) / float(size[0]);
            float value = 1;

            Vec3f color = rgb_from_hsv({ hue, saturation, value });
            rgba_array[texel++] = (char)(color[0] * 255);
            rgba_array[texel++] = (char)(color[1] * 255);
            rgba_array[texel++] = (char)(color[2] * 255);
            rgba_array[texel++] = 255;
        }
    }

//    if(texture_fill_style.get_texture()) {
//        delete texture_fill_style.get_texture();
//    }

    hue_saturation_map = new DrawableTexture((Vec2i) get_size(), rgba_array);
    delete[] rgba_array;

    texture_fill_style.set_texture(hue_saturation_map);
}

void HueSaturationSelectView::on_mouse_down(MouseDownEvent* event) {
    UIView::on_mouse_down(event);

    if(event->is_handled()) return;
    handle_mouse({ float(event->x), float(event->y) });
    event->mark_handled();
}

void HueSaturationSelectView::on_mouse_move(MouseMoveEvent* event) {
    UIView::on_mouse_move(event);

    if(event->is_handled() || !clicked) return;
    handle_mouse({ float(event->x), float(event->y) });
    event->mark_handled();
}

void HueSaturationSelectView::handle_mouse(Vec2f position) {
    position /= get_size();
    if(position[0] < 0) position.set_x(0);
    if(position[1] < 0) position.set_y(0);
    if(position[0] > 1) position.set_x(1);
    if(position[1] > 1) position.set_y(1);
    set_value(position[0] * 360, position[1]);
}

void HueSaturationSelectView::set_hue(float hue) {
    create_texture();
}


void HueSaturationSelectView::set_value(float hue, float saturation) {
    Vec2f position { hue / 360, saturation };
    position *= get_size();
    position -= selector->get_size() * 0.5;
    selector->set_position(position);
    if(callback) callback(hue, saturation);
}
