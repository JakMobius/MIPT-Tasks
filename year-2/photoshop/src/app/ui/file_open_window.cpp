//
// Created by Артем on 12.11.2021.
//

#include "file_open_window.hpp"
#include "../photoshop_view.hpp"
#include "canvas_window.hpp"
#include "alert_window.hpp"

FileOpenWindow::FileOpenWindow(PhotoshopView* app, const Vec2f& position): PhotoshopWindow(app, position, {300, 110}) {
    set_title("Open file");

    auto text = new UIText({10, 10}, {90, 40});
    input = new UIInput({110, 10}, {180, 40});
    button = new UIButton({210, 60}, {80, 40});
    button->set_title("Ok");

    button->set_callback([this]() {
        DispatchQueue::main.push(DispatchQueueTask { [this]() {
            const char* path = &input->get_contents()[0];
            try_open_file(path);
        }});
    });

    text->get_text_drawer()->set_v_alignment(VTextAlignmentCenter);
    text->get_text_drawer()->set_h_alignment(HTextAlignmentCenter);
    text->get_text_drawer()->set_text("File path");
    text->get_text_drawer()->set_font_size(17);

    get_content_view()->append_child(text);
    get_content_view()->append_child(input);
    get_content_view()->append_child(button);
}

void FileOpenWindow::try_open_file(const char* path) {
    sf::Texture image;
    if(!image.loadFromFile(path)) {
        this->app->present_window_modally(new AlertWindow("Could not open file"));
    } else {
        sf::Sprite sprite;
        sprite.setTexture(image, true);

        auto window = new CanvasWindow(app, app->get_tool_manager(), {100, 100}, { float(image.getSize().x), float(image.getSize().y) });
        window->get_canvas_view()->create_canvas({ (int)image.getSize().x, (int)image.getSize().y });
        auto canvas = window->get_canvas_view()->get_canvas();
        auto layer = new CanvasLayer(canvas->get_size());
        layer->clear_texture();

        auto layer_texture = layer->get_texture()->get_render_texture();

        layer_texture->draw(sprite);
        canvas->add_layer(layer);

        app->present_window(window);

        close();
    }
}
