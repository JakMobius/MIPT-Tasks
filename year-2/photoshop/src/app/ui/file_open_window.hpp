#pragma once

#include "photoshop_window.hpp"
#include "../../ui/ui_input.hpp"

class FileOpenWindow : public PhotoshopWindow {
    UIButton* button = nullptr;
    UIInput* input = nullptr;

public:

    explicit FileOpenWindow(PhotoshopView* app, const Vec2f& position = {});

    void try_open_file(const char* path);
};