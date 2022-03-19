//
// Created by Артем on 15.02.2022.
//

#include "collectior-guard.hpp"

CollectorGuard::CollectorGuard(StatisticsCollectorBase* base, const std::string& name) : m_base(base), m_name(name) {
    base->report_comment(m_name);
    base->increase_nesting();
}

CollectorGuard::~CollectorGuard() {
    m_base->decrease_nesting();
}
