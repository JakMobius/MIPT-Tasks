#pragma once

#include <string>
#include "logger-collector.hpp"

//#define ALLOW_SMARTVALUE_MOVE

template<typename T>
class SmartValue {

    StatisticsCollector<T>* m_statistics;
    T m_value {};
    int m_index = 0;
    std::string m_name;
    bool m_destructed = false;

    void set_tmp_name() {
        m_name = "tmp #" + std::to_string(m_statistics->get_next_value_index());
    }

public:

    const std::string& get_name() const { return m_name; }

    void set_value(T&& value) {
        m_value = value;
    }

    const T& get_value() const {
         return m_value;
    }

    SmartValue(StatisticsCollector<T>* statistics, const T& value, const std::string& name) {
        m_value = value;
        m_statistics = statistics;
        m_name = name;
        m_statistics->register_unary_operation(*this, UnaryOperatorType::construct);
    }

    SmartValue(StatisticsCollector<T>* statistics, const T& value) {
        m_value = value;
        m_statistics = statistics;
        set_tmp_name();
        m_statistics->register_unary_operation(*this, UnaryOperatorType::construct);
    }

    SmartValue(StatisticsCollector<T>* statistics, T&& value) {
        m_value = value;
        m_statistics = statistics;
        set_tmp_name();
        m_statistics->register_unary_operation(*this, UnaryOperatorType::construct);
    }

    SmartValue(StatisticsCollector<T>* statistics, T&& value, const std::string& name) {
        m_value = value;
        m_statistics = statistics;
        m_name = name;
        m_statistics->register_unary_operation(*this, UnaryOperatorType::construct);
    }

    SmartValue(const SmartValue& copy) {
        m_value = copy.m_value;
        m_statistics = copy.m_statistics;
        set_tmp_name();
        m_statistics->register_binary_operation(*this, copy, BinaryOperatorType::copy);
    }

#ifdef ALLOW_SMARTVALUE_MOVE
    SmartValue(SmartValue&& move)  noexcept {
        m_value = std::move(move.m_value);
        m_statistics = move.m_statistics;
        set_tmp_name();
        m_statistics->register_binary_operation(*this, move, BinaryOperatorType::move);
    }
#endif

    ~SmartValue() {
        m_statistics->register_unary_operation(*this, UnaryOperatorType::destruct);
    }

    SmartValue& operator=(const SmartValue& copy_assign) {
        if(&copy_assign == this) {
            m_statistics->register_binary_operation(*this, copy_assign, BinaryOperatorType::copy_assign);
            return *this;
        }

        m_value = copy_assign.m_value;
        m_statistics = copy_assign.m_statistics;
        m_statistics->register_binary_operation(*this, copy_assign, BinaryOperatorType::copy_assign);
        return *this;
    }

#ifdef ALLOW_SMARTVALUE_MOVE
    SmartValue& operator=(SmartValue&& move_assign)  noexcept {
        if(&move_assign == this) {
            m_statistics->register_binary_operation(*this, move_assign, BinaryOperatorType::move_assign);
            return *this;
        }

        m_value = std::move(move_assign.m_value);
        m_statistics = move_assign.m_statistics;
        m_statistics->register_binary_operation(*this, move_assign, BinaryOperatorType::move_assign);
        return *this;
    }
#endif
    
    bool operator<(const SmartValue& b) const {
        m_statistics->register_binary_operation(*this, b, BinaryOperatorType::less);
        return m_value < b.m_value;
    }

    bool operator>(const SmartValue& b) const {
        m_statistics->register_binary_operation(*this, b, BinaryOperatorType::greater);
        return m_value > b.m_value;
    }

    bool operator==(const SmartValue& b) const {
        m_statistics->register_binary_operation(*this, b, BinaryOperatorType::equals);
        return m_value == b.m_value;
    }

    bool operator<=(const SmartValue& b) const {
        m_statistics->register_binary_operation(*this, b, BinaryOperatorType::lequals);
        return m_value <= b.m_value;
    }

    bool operator>=(const SmartValue& b) const {
        m_statistics->register_binary_operation(*this, b, BinaryOperatorType::gequals);
        return m_value >= b.m_value;
    }

    SmartValue operator+(const SmartValue& b) const {
        SmartValue result = {m_statistics, m_value + b.get_value() };
        m_statistics->register_binary_assignment_operation(result, *this, b, BinaryAssignmentOperatorType::plus);
        return result;
    }

    SmartValue operator-(const SmartValue& b) const {
        SmartValue result = {m_statistics, m_value - b.get_value() };
        m_statistics->register_binary_assignment_operation(result, *this, b, BinaryAssignmentOperatorType::minus);
        return result;
    }

    SmartValue operator*(const SmartValue& b) const {
        SmartValue result = {m_statistics, m_value * b.get_value() };
        m_statistics->register_binary_assignment_operation(result, *this, b, BinaryAssignmentOperatorType::multiply);
        return result;
    }

    SmartValue operator/(const SmartValue& b) const {
        SmartValue result = {m_statistics, m_value / b.get_value() };
        m_statistics->register_binary_assignment_operation(result, *this, b, BinaryAssignmentOperatorType::divide);
        return result;
    }

    SmartValue operator%(const SmartValue& b) const {
        SmartValue result = {m_statistics, m_value % b.get_value() };
        m_statistics->register_binary_assignment_operation(result, *this, b, BinaryAssignmentOperatorType::modulo);
        return result;
    }

    SmartValue operator<<(const SmartValue& b) const {
        SmartValue result = {m_statistics, m_value << b.get_value() };
        m_statistics->register_binary_assignment_operation(result, *this, b, BinaryAssignmentOperatorType::lshift);
        return result;
    }

    SmartValue operator>>(const SmartValue& b) const {
        SmartValue result = {m_statistics, m_value >> b.get_value() };
        m_statistics->register_binary_assignment_operation(result, *this, b, BinaryAssignmentOperatorType::rshift);
        return result;
    }

    SmartValue operator|(const SmartValue& b) const {
        SmartValue result = {m_statistics, m_value | b.get_value() };
        m_statistics->register_binary_assignment_operation(result, *this, b, BinaryAssignmentOperatorType::bitwise_or);
        return result;
    }

    SmartValue operator&(const SmartValue& b) const {
        SmartValue result = {m_statistics, m_value ^ b.get_value() };
        m_statistics->register_binary_assignment_operation(result, *this, b, BinaryAssignmentOperatorType::bitwise_and);
        return result;
    }

    SmartValue operator^(const SmartValue& b) const {
        SmartValue result = {m_statistics, m_value ^ b.get_value() };
        m_statistics->register_binary_assignment_operation(result, *this, b, BinaryAssignmentOperatorType::bitwise_or);
        return result;
    }

    SmartValue& operator+=(const SmartValue& b) {
        m_statistics->register_binary_operation(*this, b, BinaryOperatorType::plus_inplace);
        m_value += b.get_value();
        return *this;
    }

    SmartValue& operator-=(const SmartValue& b) {
        m_statistics->register_binary_operation(*this, b, BinaryOperatorType::minus_inplace);
        m_value -= b.get_value();
        return *this;
    }

    SmartValue& operator*=(const SmartValue& b) {
        m_statistics->register_binary_operation(*this, b, BinaryOperatorType::multiply_inplace);
        m_value *= b.get_value();
        return *this;
    }

    SmartValue& operator/=(const SmartValue& b) {
        m_statistics->register_binary_operation(*this, b, BinaryOperatorType::divide_inplace);
        m_value /= b.get_value();
        return *this;
    }

    SmartValue& operator<<=(const SmartValue& b) {
        m_statistics->register_binary_operation(*this, b, BinaryOperatorType::lshift_inplace);
        m_value <<= b.get_value();
        return *this;
    }

    SmartValue operator>>=(const SmartValue& b) {
        m_statistics->register_binary_operation(*this, b, BinaryOperatorType::rshift_inplace);
        m_value <<= b.get_value();
        return *this;
    }

    SmartValue operator|=(const SmartValue& b) {
        m_statistics->register_binary_operation(*this, b, BinaryOperatorType::bitwise_or_inplace);
        m_value |= b.get_value();
        return *this;
    }

    SmartValue operator&=(const SmartValue& b) {
        m_statistics->register_binary_operation(*this, b, BinaryOperatorType::bitwise_and_inplace);
        m_value &= b.get_value();
        return *this;
    }

    SmartValue operator^=(const SmartValue& b) {
        m_statistics->register_binary_operation(*this, b, BinaryOperatorType::bitwise_xor_inplace);
        m_value ^= b.get_value();
        return *this;
    }

    SmartValue operator%=(const SmartValue& b) {
        m_statistics->register_binary_operation(*this, b, BinaryOperatorType::modulo_inplace);
        m_value %= b.get_value();
        return *this;
    }

    SmartValue operator+() {
        m_statistics->register_unary_operation(*this, UnaryOperatorType::plus);
        return SmartValue {m_statistics, +m_value };
    }

    SmartValue operator-() {
        m_statistics->register_unary_operation(*this, UnaryOperatorType::minus);
        return SmartValue {m_statistics, +m_value };
    }

    SmartValue operator~() {
        m_statistics->register_unary_operation(*this, UnaryOperatorType::inverse);
        return SmartValue {m_statistics, ~m_value };
    }

    SmartValue operator!() {
        m_statistics->register_unary_operation(*this, UnaryOperatorType::exclamation_sign);
        return SmartValue {m_statistics, !m_value };
    }

    T& get_value() {
        return m_value;
    }
};