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
//    std::cout << "SORT: assignment\n";

    DispatchQueue::main.push(DispatchQueueTask { [this] {
//        std::cout << "SORT: callback\n";
        if(m_callback) m_callback();
    }});

    m_semaphore.wait();

    if(m_kill_thread) {
        std::cout << "SORT: kill\n";
        m_kill_thread = false;
        throw SortingInterruptedException();
    }
//    std::cout << "SORT: continue\n";
}

void InterruptingCollector::register_comparsion(const SmartInt &a, const SmartInt &b) {
    StatisticsCollector::register_comparsion(a, b);

    m_last_compared_a = &a;
    m_last_compared_b = &b;

    if(m_ignore_operators) return;
//    std::cout << "SORT: comparsion\n";

    DispatchQueue::main.push(DispatchQueueTask { [this] {
//        std::cout << "SORT: callback\n";
        if(m_callback) m_callback();
    }});

    m_semaphore.wait();

    if(m_kill_thread) {
        std::cout << "SORT: kill\n";
        m_kill_thread = false;
        throw SortingInterruptedException();
    }
//    std::cout << "SORT: continue\n";
}
