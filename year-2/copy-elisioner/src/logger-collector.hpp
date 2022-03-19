#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include "statistics-collector.hpp"
#include "html-formats.hpp"

struct LoggerTheme {
    std::string m_name_color = "#fd971f";
    std::string m_ptr_color = "#ae81ff";
    std::string m_copy_op_color = "#f92672";
    std::string m_move_op_color = "#a6e22e";
    std::string m_value_color = "#ae81ff";
    std::string m_op_color = "#66d9ef";
    std::string m_comment_color = "#75715e";
    std::string m_header = "<style> html, body { background-color: #272822; min-height: 100%; color: #f8f8f2; } </style><pre>";
};

template<typename T>
struct LoggedCollector : public StatisticsCollector<T> {
    std::ofstream m_file;
    LoggerTheme m_theme {};

    int m_total_copies = 0;
    int m_total_moves = 0;

    explicit LoggedCollector(const std::string& filename): m_file(filename) {
        m_file << m_theme.m_header;
    }

    void complete() {
        write_comment("total moves: " + std::to_string(m_total_moves));
        write_comment("total copies: " + std::to_string(m_total_copies));
    }


    void padding();

    void register_unary_operation(const SmartValue<T> &operand, UnaryOperatorType operation) override;
    void register_binary_operation(const SmartValue<T> &op1, const SmartValue<T> &op2, BinaryOperatorType operation) override;

    void register_binary_assignment_operation(const SmartValue<T> &target, const SmartValue<T> &op1,
                                              const SmartValue<T> &op2,
                                              BinaryAssignmentOperatorType operation) override;

    void report_comment(const std::string &comment) override;

    void write_comment(const std::string& comment) {
        padding();
        m_file << HTMLSpanColoredPrinter(BlockCommentPrinter(comment), m_theme.m_comment_color) << "\n";
    }
};

enum class SmartValuePrintType {
    verbose,
    name_only,
    name_value
};

template<typename T>
struct SmartValuePrinter {
    const SmartValue<T>& m_value;
    LoggerTheme* m_theme;
    SmartValuePrintType m_type;

    explicit SmartValuePrinter(const SmartValue<T>& value, LoggerTheme* theme, SmartValuePrintType type):
            m_value(value), m_theme(theme), m_type(type) {}

    static SmartValuePrinter verbose(const SmartValue<T>& value, LoggerTheme* theme) {
        return SmartValuePrinter { value, theme, SmartValuePrintType::verbose };
    }

    static SmartValuePrinter name_only(const SmartValue<T>& value, LoggerTheme* theme) {
        return SmartValuePrinter { value, theme, SmartValuePrintType::name_only };
    }

    static SmartValuePrinter name_value(const SmartValue<T>& value, LoggerTheme* theme) {
        return SmartValuePrinter { value, theme, SmartValuePrintType::name_value };
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const SmartValuePrinter<T>& printer) {
    switch(printer.m_type) {
        case SmartValuePrintType::verbose:
            stream << HTMLSpanColoredPrinter(QuotedPrinter(printer.m_value.get_name()), printer.m_theme->m_name_color)
                   << " = "
                   << HTMLSpanColoredPrinter(printer.m_value.get_value(), printer.m_theme->m_value_color)
                   << " at "
                   << HTMLSpanColoredPrinter(&printer.m_value, printer.m_theme->m_ptr_color);
            break;
        case SmartValuePrintType::name_only:
            stream << HTMLSpanColoredPrinter(QuotedPrinter(printer.m_value.get_name()), printer.m_theme->m_name_color);
            break;
        case SmartValuePrintType::name_value:
            stream << "("
                   << HTMLSpanColoredPrinter(QuotedPrinter(printer.m_value.get_name()), printer.m_theme->m_name_color)
                   << " = "
                   << HTMLSpanColoredPrinter(printer.m_value.get_value(), printer.m_theme->m_value_color)
                   << ")";
            break;
    }
    return stream;
}

template<typename T>
void LoggedCollector<T>::register_unary_operation(const SmartValue<T> &operand, UnaryOperatorType operation) {
    StatisticsCollector<T>::register_unary_operation(operand, operation);

    padding();
    switch(operation) {

        case UnaryOperatorType::plus:
        case UnaryOperatorType::minus:
        case UnaryOperatorType::inverse:
        case UnaryOperatorType::exclamation_sign:
            m_file  << UnaryOperatorNames[(int)operation]
                    << SmartValuePrinter<T>::name_value(operand, &m_theme)
                    << "\n";
            break;

        case UnaryOperatorType::construct:
            m_file << HTMLSpanColoredPrinter("created ", m_theme.m_op_color) << SmartValuePrinter<T>::verbose(operand, &m_theme) << "\n";
            break;
        case UnaryOperatorType::destruct:
            m_file << HTMLSpanColoredPrinter("deleted ", m_theme.m_op_color) << SmartValuePrinter<T>::verbose(operand, &m_theme) << "\n";
            break;
    }

}

template<typename T>
void LoggedCollector<T>::register_binary_operation(const SmartValue<T> &op1, const SmartValue<T> &op2,
                                                   BinaryOperatorType operation) {
    StatisticsCollector<T>::register_binary_operation(op1, op2, operation);

    padding();
    switch(operation) {
        case BinaryOperatorType::move:
        case BinaryOperatorType::move_assign: {

            m_total_moves++;

            const char* action = operation == BinaryOperatorType::move_assign ? "move-assigned " : "move-created ";
            m_file << HTMLSpanColoredPrinter(action, m_theme.m_move_op_color)
                   << SmartValuePrinter<T>::name_only(op1, &m_theme)
                   << HTMLSpanColoredPrinter(" from ", m_theme.m_move_op_color)
                   << SmartValuePrinter<T>::name_only(op2, &m_theme)
                   << "\n";
            break;
        }
        case BinaryOperatorType::copy:
        case BinaryOperatorType::copy_assign: {

            m_total_copies++;

            const char* action = operation == BinaryOperatorType::copy_assign ? "copy-assigned " : "copy-created ";
            m_file << HTMLSpanColoredPrinter(action, m_theme.m_copy_op_color)
                   << SmartValuePrinter<T>::name_only(op1, &m_theme)
                   << HTMLSpanColoredPrinter(" from ", m_theme.m_copy_op_color)
                   << SmartValuePrinter<T>::name_only(op2, &m_theme)
                   << "\n";

            break;
        }
        case BinaryOperatorType::plus_inplace:
        case BinaryOperatorType::minus_inplace:
        case BinaryOperatorType::divide_inplace:
        case BinaryOperatorType::multiply_inplace:
        case BinaryOperatorType::modulo_inplace:
        case BinaryOperatorType::lshift_inplace:
        case BinaryOperatorType::rshift_inplace:
        case BinaryOperatorType::bitwise_or_inplace:
        case BinaryOperatorType::bitwise_and_inplace:
        case BinaryOperatorType::bitwise_xor_inplace:
        case BinaryOperatorType::less:
        case BinaryOperatorType::greater:
        case BinaryOperatorType::equals:
        case BinaryOperatorType::lequals:
        case BinaryOperatorType::gequals:

            m_file << SmartValuePrinter<T>::name_value(op1, &m_theme)
                   << " "
                   << BinaryOperatorNames[(int)operation]
                   << " "
                   << SmartValuePrinter<T>::name_value(op2, &m_theme) << "\n";

            break;
    }
}

template<typename T>
void LoggedCollector<T>::register_binary_assignment_operation(const SmartValue<T> &target, const SmartValue<T> &op1,
                                                              const SmartValue<T> &op2,
                                                              BinaryAssignmentOperatorType operation) {
    StatisticsCollector<T>::register_binary_assignment_operation(target, op1, op2, operation);

    padding();
    m_file << SmartValuePrinter<T>::name_only(target, &m_theme)
           << " = "
           << SmartValuePrinter<T>::name_value(op1, &m_theme)
           << " "
           << BinaryAssignmentOperatorNames[(int)operation]
           << " "
           << SmartValuePrinter<T>::name_value(op2, &m_theme) << "\n";
}

template<typename T>
void LoggedCollector<T>::report_comment(const std::string &comment) {
    StatisticsCollectorBase::report_comment(comment);

    write_comment(comment);
}

template<typename T>
void LoggedCollector<T>::padding() {
    for(int i = 0; i < this->m_nesting_level; i++) {
        m_file << "\t";
    }
}
