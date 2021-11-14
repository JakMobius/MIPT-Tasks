#pragma once

#include "../../ui/ui_input.hpp"
#include "dialog_window.hpp"

class FileOpenWindow : public DialogWindow {
    UIButton* button = nullptr;
    UIInput* input = nullptr;

public:

    explicit FileOpenWindow(PhotoshopView* app, const Vec2f& position = {});

    void try_open_file(const char* path);
};