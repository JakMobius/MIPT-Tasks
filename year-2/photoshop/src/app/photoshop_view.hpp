#pragma once

#include "../ui/ui_view.hpp"
#include "../ui/window/ui_window.hpp"
#include "../ui/window/ui_window_container.hpp"

class PhotoshopView : public UIWindowContainer {
public:
    explicit PhotoshopView(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}): UIWindowContainer(position, size) {

        auto* win1 = new UIWindow({100, 100}, {400, 400}, "Window 1");
        auto* win2 = new UIWindow({300, 200}, {400, 300}, "Window 2");

        add_window(win1);
        add_window(win2);

    }
};