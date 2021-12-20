#pragma once

class PhotoshopView;

class PreferencesGenerator {
public:
    explicit PreferencesGenerator() {}
    virtual ~PreferencesGenerator() {};

    virtual void open_preferences(PhotoshopView* app) = 0;
};