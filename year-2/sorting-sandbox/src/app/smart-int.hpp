#pragma once

#include "statistics-collector.hpp"

class SmartInt {

    StatisticsCollector* m_statistics;
    int m_value = 0;

public:

    void set_value(int value) {
        m_value = value;
    }

    int get_value() {
         return m_value;
    }
    
    SmartInt(StatisticsCollector* statistics, int value = 0) {
        m_value = value;
        m_statistics = statistics;
    }

    SmartInt(const SmartInt& copy) {
        m_value = copy.m_value;
        m_statistics = copy.m_statistics;
        m_statistics->register_assignment();
    }

    SmartInt(SmartInt&& move) {
        m_value = move.m_value;
        m_statistics = move.m_statistics;
        m_statistics->register_assignment();
    }

    SmartInt& operator=(const SmartInt& copy_assign) {
        if(&copy_assign == this) {
            m_statistics->register_assignment();
            return *this;
        }

        m_value = copy_assign.m_value;
        m_statistics = copy_assign.m_statistics;
        m_statistics->register_assignment();
        return *this;
    }

    SmartInt& operator=(SmartInt&& move_assign) {
        if(&move_assign == this) {
            m_statistics->register_assignment();
            return *this;
        }

        m_value = move_assign.m_value;
        m_statistics = move_assign.m_statistics;
        m_statistics->register_assignment();
        return *this;
    }
    
    bool operator<(const SmartInt& b) const {
        m_statistics->register_comparsion(*this, b);
        return m_value < b.m_value;
    }

    bool operator>(const SmartInt& b) const {
        m_statistics->register_comparsion(*this, b);
        return m_value > b.m_value;
    }

    bool operator==(const SmartInt& b) const {
        m_statistics->register_comparsion(*this, b);
        return m_value == b.m_value;
    }

    bool operator<=(const SmartInt& b) const {
        m_statistics->register_comparsion(*this, b);
        return m_value <= b.m_value;
    }

    bool operator>=(const SmartInt& b) const {
        m_statistics->register_comparsion(*this, b);
        return m_value >= b.m_value;
    }
};