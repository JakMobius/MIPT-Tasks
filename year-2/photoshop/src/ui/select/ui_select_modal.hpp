#pragma once

class UISelectModalView;

#include <vector>
#include "../ui_stack.hpp"
#include "../ui_button.hpp"
#include "./ui_select.hpp"
#include "../styles/select_view_style.hpp"

class UISelectModalView : public UIStackView {

    std::vector<UIButton*> buttons {};
    const UISelectViewStyle* style = nullptr;
public:
    UISelectModalView(std::vector<UISelectOption>* options, float width = 100);

    void set_style(const UISelectViewStyle* p_style);
};