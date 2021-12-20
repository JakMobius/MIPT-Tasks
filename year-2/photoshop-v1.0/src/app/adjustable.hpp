#pragma once

class PreferencesGenerator;

class Adjustable {
protected:
    PreferencesGenerator* preferences_generator = nullptr;

    virtual void create_preferences_generator() {}
public:
    virtual ~Adjustable();

    virtual PreferencesGenerator* get_preferences_generator() { return preferences_generator; }
};

#include "preferences_generator.hpp"