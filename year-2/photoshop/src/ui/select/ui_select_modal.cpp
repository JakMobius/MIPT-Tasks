//
// Created by Артем on 18.10.2021.
//

#include "ui_select_modal.hpp"
#include "../ui_stack.hpp"

UISelectModalView::UISelectModalView(std::vector<UISelectOption>* options, float width): UIStackView(UIStackViewDirection::y, {}) {
    for(int i = 0; i < options->size(); i++) {
        auto* button = new UIButton({}, { width, 50 });
        button->set_title((*options)[i].title);
        buttons.push_back(button);
        append_child(button);
    }
}

void UISelectModalView::set_style(const UISelectViewStyle* p_style) {
    style = p_style;
    for(int i = 0; i < buttons.size(); i++) {
        (*buttons[i]).set_style(style->get_button_style());
    }
}
