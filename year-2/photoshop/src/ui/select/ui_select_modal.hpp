#pragma once

class UISelectModalView;

#include <vector>
#include "../ui_stack.hpp"
#include "../ui_button.hpp"
#include "./ui_select.hpp"

class UISelectModalView : public UIStackView {

public:
    UISelectModalView(std::vector<UISelectOption>* options, float width = 100);
};