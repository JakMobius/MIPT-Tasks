#pragma once

class AppView;

#include "../../ui/window/ui_window.hpp"

class AppWindow: public UIWindow {
protected:
    AppView* app;
public:
    explicit AppWindow(AppView* app, const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}, const char* title = nullptr):
        UIWindow(position, size, title),
        app(app) {};

    AppView* get_app() { return app; }
};

#include "../app-view.hpp"