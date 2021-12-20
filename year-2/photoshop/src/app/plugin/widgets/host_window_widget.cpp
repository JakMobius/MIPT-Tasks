//
// Created by Артем on 15.12.2021.
//

#include "host_window_widget.hpp"
#include "../app_interface.hpp"
#include "../proxy_window.hpp"

void HostWindow::set_show_handler(PUPPY::Window::HandlerType &handler_) {
    show_handler = handler_;
}

PUPPY::Window::HandlerType &HostWindow::get_show_handler() {
    return show_handler;
}

void HostWindow::set_hide_handler(PUPPY::Window::HandlerType &handler_) {
    hide_handler = handler_;
}

PUPPY::Window::HandlerType &HostWindow::get_hide_handler() {
    return hide_handler;
}

bool HostWindow::set_name(const char* name) {
    ((UIWindow*)view)->set_title(name);
    return true;
}

const char* HostWindow::get_name() {
    return nullptr;
}

HostWindow::HostWindow(const PUPPY::WBody &body, PUPPY::Widget* parent, const char* title) : RawHostWidget(body, parent) {

    nested_container = new HostWidget({{}, body.size}, nullptr);

    auto window = new ProxyWindow(HostAppInterface::pvec2f_to_vec2f(body.position), HostAppInterface::pvec2f_to_vec2f(body.size), title);
    window->set_position({body.position.x, body.position.y});
    window->set_size({body.size.x, body.size.y});
    window->set_can_be_closed(false);
    window->get_content_view()->append_child(nested_container->get_view());
    set_view(window);

    if(parent) {
        parent->add_child(this);
    }
}

bool HostWindow::add_child(PUPPY::Widget* child) {
    nested_container->add_child(child);
    return true;
}

void HostWindow::on_render(const PUPPY::Event::Render &event) {
    nested_container->get_view()->set_needs_redraw();
    RawHostWidget::on_render(event);
}
