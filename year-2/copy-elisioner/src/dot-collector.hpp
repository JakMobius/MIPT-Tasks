#pragma once

#include <map>
#include <set>
#include "statistics-collector.hpp"

//struct LoggerTheme {
//    std::string m_name_color = "#fd971f";
//    std::string m_ptr_color = "#ae81ff";
//    std::string m_copy_op_color = "#f92672";
//    std::string m_move_op_color = "#a6e22e";
//    std::string m_value_color = "#ae81ff";
//    std::string m_op_color = "#66d9ef";
//    std::string m_comment_color = "#75715e";
//    std::string m_header = "<style> html, body { background-color: #272822; min-height: 100%; color: #f8f8f2; } </style><pre>";
//};

struct DotCollectorTheme {
    std::string m_header = "digraph G {\n"
                           "\tgraph [ fontname=monospace, rankdir=\"TB\", bgcolor=\"#272822\" fontcolor=\"#f8f8f2\" ]\n"
                           "\tnode [ fontname=monospace, color=\"#f8f8f2\" fontcolor=\"#f8f8f2\" ]\n"
                           "\tedge [ fontname=monospace, color=\"#f8f8f2\" fontcolor=\"#f8f8f2\" ]\n";
    std::string m_footer = "}\n";
    std::string m_copy_node_attributes = "fontcolor=\"#f92672\"";
    std::string m_move_node_attributes = "fontcolor=\"#a6e22e\"";
    std::string m_value_text_color = "#ae81ff";
    std::string m_name_text_color = "#fd971f";
    std::string m_destructed_text_color = "#f92672";
};

struct DotNode {
    int m_index;
    const char* m_prefix;
    DotNode(int index, const char* prefix): m_index(index), m_prefix(prefix) {};
};

struct DotOperator : public DotNode {
    DotOperator(int index): DotNode(index, "oper") {};
};

struct DotOperand : public DotNode {
    DotOperand(int index): DotNode(index, "var") {};
};

std::ostream& operator<<(std::ostream& stream, const DotNode& node);

struct DotAttributeDrawer {
    std::ostream& m_stream;
    bool m_any_attributes = false;

    DotAttributeDrawer(std::ostream& stream): m_stream(stream) {}

    ~DotAttributeDrawer() {
        if(m_any_attributes) m_stream << "]";
        m_stream << "\n";
    }

    template<typename T>
    DotAttributeDrawer& operator<<(const T& attribute) {
        if(!m_any_attributes) {
            m_stream << "[";
            m_any_attributes = true;
        }
        m_stream << attribute;
        return *this;
    }
};

template<typename T>
class DotCollector : public StatisticsCollector<T> {
    std::ofstream m_file {};
    DotCollectorTheme m_theme {};

    std::map<const SmartValue<T>*, int> m_node_identifiers {};

    int total_identifiers = 0;
    int total_operators = 0;

public:
    explicit DotCollector(const std::string& filename): m_file(filename) {
        m_file << m_theme.m_header;
    }

    void complete() {
        m_file << m_theme.m_footer;
    }

    void register_unary_operation(const SmartValue<T> &operand, UnaryOperatorType operation) override;
    void register_binary_operation(const SmartValue<T> &op1, const SmartValue<T> &op2, BinaryOperatorType operation) override;

    void register_binary_assignment_operation(const SmartValue<T> &target, const SmartValue<T> &op1,
                                              const SmartValue<T> &op2,
                                              BinaryAssignmentOperatorType operation) override;

    void report_comment(const std::string &comment) override;

    void padding() {
        m_file << "\t";
    }

    DotAttributeDrawer declare(DotNode& node) {
        m_file << "\t" << node;
        return DotAttributeDrawer(m_file);
    }

    DotAttributeDrawer arrow(DotNode old_node, DotNode new_node) {
        m_file << "\t" << old_node << " -> " << new_node;
        return DotAttributeDrawer(m_file);
    }

    void timeline_link(DotOperand old_node, DotOperand new_node) {
        arrow(old_node, new_node) << "style=dashed";
    }

    DotOperand get_variable_node(const SmartValue<T>* ptr) {
        auto it = m_node_identifiers.find(ptr);
        if(it == m_node_identifiers.end()) {
            return DotOperand(set_new_identifier(ptr));
        } else {
            return DotOperand(it->second);
        }
    }

    DotOperand set_new_identifier(const SmartValue<T>* ptr, bool destructed = false) {
        auto result = DotOperand(total_identifiers);
        auto attributes = declare(result);
        attributes << "label=<"
                << HTMLFontColoredPrinter(QuotedPrinter(ptr->get_name()), m_theme.m_name_text_color) << "<br/>"
                << HTMLFontColoredPrinter(ptr->get_value(), m_theme.m_value_text_color) << "<br/>"
                << HTMLFontColoredPrinter(ptr, m_theme.m_value_text_color) << "<br/>";
        if(destructed) {
            attributes << HTMLFontColoredPrinter("destructed", m_theme.m_destructed_text_color) << "<br/>";
        }
        attributes << ">, shape=rectangle";

        m_node_identifiers[ptr] = total_identifiers++;
        return result;
    }

    DotOperand recreate_node(const SmartValue<T>* ptr, bool destructed = false) {

        auto it = m_node_identifiers.find(ptr);
        if(it == m_node_identifiers.end()) {
            return set_new_identifier(ptr, destructed);
        }

        int old_node = it->second;
        auto new_node = set_new_identifier(ptr, destructed);
        timeline_link(old_node, new_node);
        return new_node;
    }

    DotOperator register_operator(const std::string& name, const std::string& attributes = "") {
        auto result = DotOperator(total_operators++);
        declare(result) << "label=" << QuotedPrinter(name) << " shape=diamond " << attributes;
        return result;
    }
};

template<typename T>
void DotCollector<T>::register_unary_operation(const SmartValue<T> &operand, UnaryOperatorType operation) {
    StatisticsCollector<T>::register_unary_operation(operand, operation);

    if(operation == UnaryOperatorType::construct) return;

    auto operand_node = get_variable_node(&operand);

    if(operation == UnaryOperatorType::destruct) {
        recreate_node(&operand, true);
        m_node_identifiers.erase(&operand);
        return;
    } else {
        auto operator_index = register_operator(UnaryOperatorNames[(int)operation]);
        arrow(operand_node, operator_index);
        operand_node = recreate_node(&operand);
        arrow(operator_index, operand_node);
    }
}

template<typename T>
void DotCollector<T>::register_binary_operation(const SmartValue<T> &op1, const SmartValue<T> &op2,
                                                BinaryOperatorType operation) {
    StatisticsCollector<T>::register_binary_operation(op1, op2, operation);

    if(operation == BinaryOperatorType::copy || operation == BinaryOperatorType::move) {
//        m_destructed_identifiers.erase(&op1);
    }

    std::string attributes;
    std::string name = BinaryOperatorNames[(int)operation];

    if(operation == BinaryOperatorType::copy || operation == BinaryOperatorType::copy_assign) {
        attributes = m_theme.m_copy_node_attributes;
        name = "copy";
    } else if(operation == BinaryOperatorType::move || operation == BinaryOperatorType::move_assign) {
        attributes = m_theme.m_move_node_attributes;
        name = "move";
    }

    auto operator_node = register_operator(name, attributes);
    arrow(get_variable_node(&op2), operator_node);
    arrow(operator_node, recreate_node(&op1));
}

template<typename T>
void DotCollector<T>::register_binary_assignment_operation(const SmartValue<T> &target, const SmartValue<T> &op1,
                                                           const SmartValue<T> &op2,
                                                           BinaryAssignmentOperatorType operation) {
    StatisticsCollector<T>::register_binary_assignment_operation(target, op1, op2, operation);

    auto operator_node = register_operator(BinaryAssignmentOperatorNames[(int)operation]);

    arrow(get_variable_node(&op1), operator_node);
    arrow(get_variable_node(&op2), operator_node);
    arrow(operator_node, recreate_node(&target));
}

template<typename T>
void DotCollector<T>::report_comment(const std::string &comment) {
    StatisticsCollectorBase::report_comment(comment);

}
