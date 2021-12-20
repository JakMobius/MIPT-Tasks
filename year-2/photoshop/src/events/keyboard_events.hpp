#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <cstdint>
#include "event.hpp"

enum class KeyCode {
    a            = sf::Keyboard::A,
    b            = sf::Keyboard::B,
    c            = sf::Keyboard::C,
    d            = sf::Keyboard::D,
    e            = sf::Keyboard::E,
    f            = sf::Keyboard::F,
    g            = sf::Keyboard::G,
    h            = sf::Keyboard::H,
    i            = sf::Keyboard::I,
    j            = sf::Keyboard::J,
    k            = sf::Keyboard::K,
    l            = sf::Keyboard::L,
    m            = sf::Keyboard::M,
    n            = sf::Keyboard::N,
    o            = sf::Keyboard::O,
    p            = sf::Keyboard::P,
    q            = sf::Keyboard::Q,
    r            = sf::Keyboard::R,
    s            = sf::Keyboard::S,
    t            = sf::Keyboard::T,
    u            = sf::Keyboard::U,
    v            = sf::Keyboard::V,
    w            = sf::Keyboard::W,
    x            = sf::Keyboard::X,
    y            = sf::Keyboard::Y,
    z            = sf::Keyboard::Z,
    num0         = sf::Keyboard::Num0,
    num1         = sf::Keyboard::Num1,
    num2         = sf::Keyboard::Num2,
    num3         = sf::Keyboard::Num3,
    num4         = sf::Keyboard::Num4,
    num5         = sf::Keyboard::Num5,
    num6         = sf::Keyboard::Num6,
    num7         = sf::Keyboard::Num7,
    num8         = sf::Keyboard::Num8,
    num9         = sf::Keyboard::Num9,
    escape       = sf::Keyboard::Escape,
    lcontrol     = sf::Keyboard::LControl,
    lshift       = sf::Keyboard::LShift,
    lalt         = sf::Keyboard::LAlt,
    lsystem      = sf::Keyboard::LSystem,
    rcontrol     = sf::Keyboard::RControl,
    rshift       = sf::Keyboard::RShift,
    ralt         = sf::Keyboard::RAlt,
    rsystem      = sf::Keyboard::RSystem,
    menu         = sf::Keyboard::Menu,
    lbracket     = sf::Keyboard::LBracket,
    rbracket     = sf::Keyboard::RBracket,
    semicolon    = sf::Keyboard::Semicolon,
    comma        = sf::Keyboard::Comma,
    period       = sf::Keyboard::Period,
    quote        = sf::Keyboard::Quote,
    slash        = sf::Keyboard::Slash,
    backslash    = sf::Keyboard::Backslash,
    tilde        = sf::Keyboard::Tilde,
    equal        = sf::Keyboard::Equal,
    hyphen       = sf::Keyboard::Hyphen,
    space        = sf::Keyboard::Space,
    enter        = sf::Keyboard::Enter,
    backspace    = sf::Keyboard::Backspace,
    tab          = sf::Keyboard::Tab,
    pageup       = sf::Keyboard::PageUp,
    pagedown     = sf::Keyboard::PageDown,
    end          = sf::Keyboard::End,
    home         = sf::Keyboard::Home,
    insert       = sf::Keyboard::Insert,
    del          = sf::Keyboard::Delete,
    add          = sf::Keyboard::Add,
    subtract     = sf::Keyboard::Subtract,
    multiply     = sf::Keyboard::Multiply,
    divide       = sf::Keyboard::Divide,
    left         = sf::Keyboard::Left,
    right        = sf::Keyboard::Right,
    up           = sf::Keyboard::Up,
    down         = sf::Keyboard::Down,
    numpad0      = sf::Keyboard::Numpad0,
    numpad1      = sf::Keyboard::Numpad1,
    numpad2      = sf::Keyboard::Numpad2,
    numpad3      = sf::Keyboard::Numpad3,
    numpad4      = sf::Keyboard::Numpad4,
    numpad5      = sf::Keyboard::Numpad5,
    numpad6      = sf::Keyboard::Numpad6,
    numpad7      = sf::Keyboard::Numpad7,
    numpad8      = sf::Keyboard::Numpad8,
    numpad9      = sf::Keyboard::Numpad9,
    f1           = sf::Keyboard::F1,
    f2           = sf::Keyboard::F2,
    f3           = sf::Keyboard::F3,
    f4           = sf::Keyboard::F4,
    f5           = sf::Keyboard::F5,
    f6           = sf::Keyboard::F6,
    f7           = sf::Keyboard::F7,
    f8           = sf::Keyboard::F8,
    f9           = sf::Keyboard::F9,
    f10          = sf::Keyboard::F10,
    f11          = sf::Keyboard::F11,
    f12          = sf::Keyboard::F12,
    f13          = sf::Keyboard::F13,
    f14          = sf::Keyboard::F14,
    f15          = sf::Keyboard::F15,
    pause        = sf::Keyboard::Pause,
};

struct TextEnterEvent: public EventBase {
    uint32_t unicode;

    TextEnterEvent(uint32_t unicode): EventBase(), unicode(unicode) {};

    bool is_ascii() {
        return !(unicode & 0xFF00);
    }

    char get_ascii() {
        if(!is_ascii()) return 0;
        else return (char) unicode;
    }

    uint32_t get_unicode() { return unicode; }
};

struct KeyEvent : public EventBase {
    KeyCode code;
    explicit KeyEvent(KeyCode code): EventBase(), code(code) {}
};

struct KeyDownEvent : public KeyEvent {
    bool control = false;
    bool shift = false;
    bool alt = false;
    bool super = false;

    KeyDownEvent(KeyCode code): KeyEvent(code) {}
};
struct KeyUpEvent : public KeyEvent {
    KeyUpEvent(KeyCode code): KeyEvent(code) {}
};