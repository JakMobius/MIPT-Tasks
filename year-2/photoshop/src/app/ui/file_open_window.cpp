//
// Created by Артем on 12.11.2021.
//

#include "file_open_window.hpp"
#include "../photoshop_view.hpp"
#include "canvas_window.hpp"
#include "alert_window.hpp"

FileOpenWindow::FileOpenWindow(PhotoshopView* app, const Vec2f& position): DialogWindow(app, position, {400, 110}) {
    set_title("Open file");

    text_width = 70;
    input = create_string_field("File path");

    button = new UIButton({}, {80, 40});
    button->set_title("Ok");
    stack->append_child(button);

    button->set_callback([this]() {
        DispatchQueue::main.push(DispatchQueueTask {[this]() {
            try_open_file(&input->get_contents()[0]);
        }});
    });

    get_content_view()->append_child(stack);
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
