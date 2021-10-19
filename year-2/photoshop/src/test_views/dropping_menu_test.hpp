#pragma once

#include "../ui/ui_view.hpp"
#include "../ui/ui_stack.hpp"
#include "../ui/select/ui_select.hpp"

class DroppingMenuTestView : public UIView {

    void create_menu() {
        auto* stack = new UIStackView(UIStackViewDirection::y);
        append_child(stack);
        stack->set_primary_alignment(UIStackViewPrimaryAlignment::center);
        stack->set_lateral_alignment(UIStackViewLateralAlignment::center);
        stack->set_fitting({get_size()[0], get_size()[1]});

        auto* select = new UISelectView();
        select->add_variant({ "Option 1" });
        select->add_variant({ "Option 2" });
        select->add_variant({ "Option 3" });
        select->add_variant({ "Option 4" });
        stack->append_child(select);
    }

public:
    DroppingMenuTestView(const Vec2f &position = {0, 0}, const Vec2f &size = {0, 0}): UIView(position, size) {
        create_menu();
    }
};