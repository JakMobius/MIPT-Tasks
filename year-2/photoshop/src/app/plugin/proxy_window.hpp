#pragma once

#include "../../ui/window/ui_window.hpp"
#include "raw_host_widget.hpp"

class ProxyWindow : public UIWindow {
public:
    explicit ProxyWindow(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}, const char* title = nullptr): UIWindow(position, size, title) {
        inner_view->set_cache_texture(false);
    }




};