#pragma once

#include <thread>
#include "../ui/ui_view.hpp"
#include "statistics-collector.hpp"
#include "smart-int.hpp"

class AnimationView : public UIView {
    std::vector<SmartInt> m_array {};
    std::atomic<bool> m_task_busy { false };
    int m_array_max = 32;

    InterruptingCollector m_collector;

    std::thread* m_thread = nullptr;
    bool m_sorting_is_waiting = false;

public:
    AnimationView(const Vec2f& position, const Vec2f& size);
    ~AnimationView() override;

    void perform_task_async(const std::function<void(void)>& task);
    std::vector<SmartInt>& get_array() { return m_array; };

    InterruptingCollector& get_statistics_collector() { return m_collector; }

    static void async_task_entry(AnimationView* view, const std::function<void(void)>& func);

    void draw(DrawingContext *ctx) override;
};