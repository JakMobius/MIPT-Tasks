#pragma once

#include "../../ui/window/ui_window.hpp"
#include "../../utils/dispatch_queue.hpp"

class AlertWindow : public UIWindow {
    UIFillStyleTexture* alert_texture = new UIFillStyleTexture(Assets.error_texture.get());
public:
    explicit AlertWindow(const char* text);
};