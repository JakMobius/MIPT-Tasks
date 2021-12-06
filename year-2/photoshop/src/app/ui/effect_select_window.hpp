#pragma once

//
// Created by Артем on 01.12.2021.
//

#include "photoshop_window.hpp"

class EffectSelectWindow : public PhotoshopWindow {
    UIStackView* stack = nullptr;

    void create_tool_buttons();

public:
    explicit EffectSelectWindow(PhotoshopView* app, const Vec2f& position);

    void layout() override;
};