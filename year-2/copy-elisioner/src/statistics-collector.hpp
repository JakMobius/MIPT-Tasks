#pragma once

struct StatisticsCollectorBase;

#include <string>
#include "collectior-guard.hpp"

#define COLLECTOR_FUNC(collector) auto __collector_guard = collector.guard(std::string("function: ") + __func__);
#define SMART_VALUE(collector, name, value) auto name = collector.create(value, #name);

template<typename T>
class SmartValue;

enum class UnaryOperatorType {
    plus,
    minus,
    inverse,
    exclamation_sign,
    construct,
    destruct
};

enum class BinaryOperatorType {
    plus_inplace,
    minus_inplace,
    divide_inplace,
    multiply_inplace,
    modulo_inplace,
    lshift_inplace,
    rshift_inplace,
    bitwise_or_inplace,
    bitwise_and_inplace,
    bitwise_xor_inplace,
    move,
    copy,
    move_assign,
    copy_assign,
    less,
    greater,
    equals,
    lequals,
    gequals
};

enum class BinaryAssignmentOperatorType {
    plus,
    minus,
    divide,
    multiply,
    modulo,
    lshift,
    rshift,
    bitwise_or,
    bitwise_and,
    bitwise_xor,
};

extern const char* UnaryOperatorNames[];
extern const char* BinaryOperatorNames[];
extern const char* BinaryAssignmentOperatorNames[];

class StatisticsCollectorBase {
protected:
    int m_nesting_level = 0;

public:

    void increase_nesting() { m_nesting_level++; }
    void decrease_nesting() { m_nesting_level--; }

    virtual void report_comment(const std::string& comment) {};

    CollectorGuard guard(const std::string& name);
};

template<typename T>
class StatisticsCollector : public StatisticsCollectorBase {
protected:
    int m_values = 0;
public:
    int get_next_value_index() { return m_values++; };

    virtual void register_unary_operation(const SmartValue<T>& operand, UnaryOperatorType operation) {};
    virtual void register_binary_operation(const SmartValue<T>& op1, const SmartValue<T>& op2, BinaryOperatorType operation) {};
    virtual void register_binary_assignment_operation(const SmartValue<T>& target, const SmartValue<T>& op1, const SmartValue<T>& op2, BinaryAssignmentOperatorType operation) {};

    SmartValue<T> create(const T& value, const std::string& name);
    SmartValue<T> create(T&& value, const std::string& name);
};

#include "smart-value.hpp"
template<typename T>
SmartValue<T> StatisticsCollector<T>::create(T &&value, const std::string& name) {
    return { this, value, name };
}

template<typename T>
SmartValue<T> StatisticsCollector<T>::create(const T &value, const std::string& name) {
    return { this, value, name };
}

