#pragma once

class SmartInt;

#include <vector>
#include "../utils/semaphores.h"

struct StatisticsCollector {
    int m_assignments = 0;
    int m_comparisons = 0;

    virtual void register_assignment();
    virtual void register_comparsion(const SmartInt& a, const SmartInt& b);
};

struct KindaException : public std::exception {
    size_t m_code;
    KindaException(size_t code) noexcept: m_code(code) {};
    char const* what() const noexcept override { return "Sorting was interrupted"; };
};

struct SortingInterruptedException : public std::exception {
    SortingInterruptedException() noexcept = default;
    char const* what() const noexcept override { return "Sorting was interrupted"; };
};

struct InterruptingCollector : public StatisticsCollector {
    Semaphore m_semaphore { 0 };
    std::function<void(void)> m_callback {};
    std::atomic<bool> m_kill_thread { false };
    std::atomic<bool> m_ignore_operators { false };

    std::atomic<const SmartInt*> m_last_compared_a { nullptr };
    std::atomic<const SmartInt*> m_last_compared_b { nullptr };

    InterruptingCollector() {

    }

    void kill_sorting() {
        m_kill_thread = true;
        continue_sorting();
    }

    void continue_sorting() {
        m_semaphore.post();
    }

    void set_callback(const std::function<void(void)>& callback) {
        m_callback = callback;
    };

    void hold_on();

    void register_assignment() override;
    void register_comparsion(const SmartInt& a, const SmartInt& b) override;
};