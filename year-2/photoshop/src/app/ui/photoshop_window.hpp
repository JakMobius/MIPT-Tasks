#pragma once

class PhotoshopView;

#include "../../ui/window/ui_window.hpp"

class PhotoshopWindow: public UIWindow {
    PhotoshopView* app;
public:
    explicit PhotoshopWindow(PhotoshopView* app, const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}, const char* title = nullptr):
        UIWindow(position, size, title),
        app(app) {};

    PhotoshopView* get_app() { return app; }
};

#include "../photoshop_view.hpp"