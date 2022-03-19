#pragma once

class CollectorGuard;

#include "statistics-collector.hpp"

struct CollectorGuard {
    std::string m_name;
    StatisticsCollectorBase* m_base;
    CollectorGuard(StatisticsCollectorBase* base, const std::string& name);

    ~CollectorGuard();
};