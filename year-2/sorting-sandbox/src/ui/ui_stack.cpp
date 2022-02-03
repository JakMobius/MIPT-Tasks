//
// Created by Артем on 17.10.2021.
//

#include "ui_stack.hpp"

void UIStackView::layout_primary_leading(UIAxis p_direction, float from, float to) {
    for(auto child : m_children) {
        Vec2f child_position = child->get_position();
        Vec2f child_size = child->get_size();
        child_position.set((int) p_direction, from);
        child->set_position(child_position);
        from += child_size[(int) p_direction] + item_spacing;
    }
}

void UIStackView::layout_primary_trailing(UIAxis p_direction, float from, float to) {
    for(int i = (int)m_children.size() - 1; i >= 0; i--) {
        auto child = m_children[i];
        Vec2f child_position = child->get_position();
        Vec2f child_size = child->get_size();
        child_position.set((int) p_direction, to - child_size[(int) p_direction]);
        child->set_position(child_position);
        to -= child_size[(int) p_direction] + item_spacing;
    }
}

void UIStackView::layout_primary_center(UIAxis p_direction, float from, float to) {
    if(m_children.empty()) return;

    float total_size = 0;

    for(int i = 0; i < m_children.size(); i++) {
        total_size += m_children[i]->get_size()[(int) p_direction] + item_spacing;
    }

    from = (to + from - total_size + item_spacing) / 2;

    for(int i = 0; i < m_children.size(); i++) {
        auto child = m_children[i];
        Vec2f child_position = child->get_position();
        Vec2f child_size = child->get_size();
        child_position.set((int) p_direction, from);
        child->set_position(child_position);
        from += child_size[(int) p_direction] + item_spacing;
    }
}

void UIStackView::layout_primary_space(UIAxis p_direction, float from, float to, UIStackViewSpaceMode mode) {
    if(m_children.empty()) return;

    float total_size = 0;

    for(int i = 0; i < m_children.size(); i++) {
        auto child = m_children[i];
        total_size += child->get_size()[(int) p_direction];
    }

    float free_size = to - from - total_size;
    if(mode == UIStackViewSpaceMode::around) {
        float padding = free_size / float(m_children.size()) / 2;
        from += padding;
        free_size -= padding * 2;
    } else if(mode == UIStackViewSpaceMode::evenly) {
        float padding = free_size / float(m_children.size() + 1);
        from += padding;
        free_size -= padding * 2;
    }

    float padding = free_size / float(m_children.size() - 1);

    for(int i = 0; i < m_children.size(); i++) {
        auto child = m_children[i];
        Vec2f child_position = child->get_position();
        Vec2f child_size = child->get_size();
        child_position.set((int) p_direction, from);
        child->set_position(child_position);
        from += child_size[(int) p_direction] + padding;
    }
}

void UIStackView::layout_primary(UIAxis p_direction, float from, float to) {
    switch(primary_alignment) {
        case UIStackViewPrimaryAlignment::leading:  layout_primary_leading (p_direction, from, to); break;
        case UIStackViewPrimaryAlignment::trailing: layout_primary_trailing(p_direction, from, to); break;
        case UIStackViewPrimaryAlignment::center:   layout_primary_center  (p_direction, from, to); break;
        case UIStackViewPrimaryAlignment::space_between: layout_primary_space(p_direction, from, to, UIStackViewSpaceMode::between); break;
        case UIStackViewPrimaryAlignment::space_around:  layout_primary_space(p_direction, from, to, UIStackViewSpaceMode::around);  break;
        case UIStackViewPrimaryAlignment::space_evenly:  layout_primary_space(p_direction, from, to, UIStackViewSpaceMode::evenly);  break;
    }
}

void UIStackView::layout_lateral(UIAxis p_direction, float from, float to) {
    for(int i = 0; i < m_children.size(); i++) {
        auto child = m_children[i];
        float size = child->get_size()[(int) p_direction];
        float position = 0;

        switch(lateral_alignment) {
            case UIStackViewLateralAlignment::leading:  position = from;                   break;
            case UIStackViewLateralAlignment::trailing: position = to - size;              break;
            case UIStackViewLateralAlignment::center:   position = (to + from - size) / 2; break;
        }

        Vec2f child_position = child->get_position();
        child_position.set((int) p_direction, position);
        child->set_position(child_position);
    }
}

float UIStackView::layout_direction(UIAxis p_direction) {
    float size = 0;
    if(p_direction == direction) size = get_primary_size(p_direction);
    else size = get_lateral_size(p_direction);

    float leading_inset = insets.get_leading_inset(p_direction, size);
    float trailing_inset = insets.get_trailing_inset(p_direction, size);

    auto& dir_fitting = fitting.by_direction(p_direction);
    if(dir_fitting.is_fitting) size += leading_inset + trailing_inset;

    float start = leading_inset;
    float end = size - trailing_inset;

    if(p_direction == direction) layout_primary(p_direction, start, end);
    else layout_lateral(p_direction, start, end);

    return size;
}

float UIStackView::get_primary_size(UIAxis p_direction) {
    auto& dir_fitting = fitting.by_direction(p_direction);
    if(!dir_fitting.is_fitting) return dir_fitting.size;

    float total_size = 0;
    for(auto & i : m_children) {
        total_size += i->get_size()[(int) p_direction];
    }

    switch(primary_alignment) {
        case UIStackViewPrimaryAlignment::center:
        case UIStackViewPrimaryAlignment::leading:
        case UIStackViewPrimaryAlignment::trailing:
            if(!m_children.empty()) {
                total_size += item_spacing * float(m_children.size() - 1);
            }
            break;
        default: break;
    }

    return total_size;
}

float UIStackView::get_lateral_size(UIAxis p_direction) {
    auto& dir_fitting = fitting.by_direction(p_direction);
    if(!dir_fitting.is_fitting) return dir_fitting.size;

    float total_size = 0;

    for(int i = 0; i < m_children.size(); i++) {
        auto child = m_children[i];
        total_size = std::max(total_size, child->get_size()[(int) p_direction]);
    }

    return total_size;
}
