#pragma once

template <typename StyleClass>
class Styled {
protected:
    const StyleClass* style = nullptr;
    bool style_owned = false;
public:
    ~Styled() { if(style_owned) delete style; }
    virtual void set_style(const StyleClass* p_style) {
        style = p_style;
    }

    void set_own_style(const StyleClass* p_style) {
        set_style(p_style);
        style_owned = true;
    }

    const StyleClass* get_style() const { return style; }
};