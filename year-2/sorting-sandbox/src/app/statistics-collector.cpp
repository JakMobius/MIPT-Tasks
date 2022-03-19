//
// Created by Артем on 03.02.2022.
//

#include "statistics-collector.hpp"
#include "../utils/dispatch_queue.hpp"
#include <iostream>

void StatisticsCollector::register_assignment() {
    m_assignments++;
}

void StatisticsCollector::register_comparsion(const SmartInt&, const SmartInt&) {
    m_comparisons++;
}

void InterruptingCollector::register_assignment() {
    StatisticsCollector::register_assignment();

    if(m_ignore_operators) return;

    hold_on();
}

void InterruptingCollector::register_comparsion(const SmartInt &a, const SmartInt &b) {
    StatisticsCollector::register_comparsion(a, b);

    m_last_compared_a = &a;
    m_last_compared_b = &b;

    if(m_ignore_operators) return;

    hold_on();
}

void InterruptingCollector::hold_on() {
    DispatchQueue::main.push(DispatchQueueTask { [this] {
        if(m_callback) m_callback();
    }});

    m_semaphore.wait();

    if(m_kill_thread) {
        m_kill_thread = false;
        throw SortingInterruptedException();
    }
}
