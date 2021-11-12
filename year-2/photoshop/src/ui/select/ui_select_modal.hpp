#pragma once

class UISelectModalView;

#include <vector>
#include "../ui_stack.hpp"
#include "../ui_button.hpp"
#include "./ui_select.hpp"
#include "../styles/select_view_style.hpp"
#include "../ui_modal_view.hpp"

class UISelectModalView : public UIModalView {

    std::vector<UIButton*> buttons {};
    const UISelectViewStyle* style = nullptr;
    float width;
public:
    UISelectModalView(UIModalViewPresentModeDropdown* mode, std::vector<UISelectOption>* options, float width = 100);

    void set_style(const UISelectViewStyle* p_style);
    void layout() override;
};