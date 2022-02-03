#pragma once

class AppView;

class PreferencesGenerator {
public:
    explicit PreferencesGenerator() {}
    virtual ~PreferencesGenerator() {};

    virtual void open_preferences(AppView* app) = 0;
};