//
// Created by Артем on 20.10.2021.
//

#include "window_style.hpp"

static UIDefaultCloseButtonStyle UIDefaultCloseButtonStyleInstance {};
UIDefaultCloseButtonStyle* UIDefaultCloseButtonStyle::instance = &UIDefaultCloseButtonStyleInstance;

static UIDefaultFullscreenButtonStyle UIDefaultFullscreenButtonStyleInstance {};
UIDefaultFullscreenButtonStyle* UIDefaultFullscreenButtonStyle::instance = &UIDefaultFullscreenButtonStyleInstance;

static UIWindowStyle UIWindowStyleInstance {};
UIWindowStyle* UIWindowStyle::instance = &UIWindowStyleInstance;