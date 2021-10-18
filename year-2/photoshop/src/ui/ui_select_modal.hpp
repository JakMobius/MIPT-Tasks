#pragma once

class UISelectModalView;

#include "../utils/dynamic_array.hpp"
#include "./ui_stack.hpp"
#include "./ui_button.hpp"
#include "./ui_select.hpp"

class UISelectModalView : public UIStackView {

public:
    UISelectModalView(dynamic_array<UISelectOption>* options, float width = 100);
};