//
// Created by Артем on 04.02.2022.
//

#include <random>
#include "animation-view.hpp"

AnimationView::AnimationView(const Vec2f &position, const Vec2f &size) : UIView(position, size) {
    m_collector.m_ignore_operators = true;

    for(int i = 0; i < m_array_max; i++) {
        m_array.emplace_back(&m_collector, i);
    }

    m_collector.m_ignore_operators = false;

    m_collector.set_callback([this]() {
        atomic_thread_fence(std::memory_order_acquire);
        set_needs_redraw();
        m_sorting_is_waiting = true;
    });
}

void AnimationView::perform_task_async(const std::function<void(void)>& task) {
    if(m_thread) {
        if(m_task_busy) {
            m_collector.kill_sorting();
            if(m_thread->joinable()) m_thread->join();
            m_collector.m_kill_thread = false;
            m_sorting_is_waiting = false;
            m_task_busy = false;
        }

        delete m_thread;
        m_thread = nullptr;
    }
    m_task_busy = true;
    m_thread = new std::thread(AnimationView::async_task_entry, this, task);
}

void AnimationView::draw(DrawingContext* ctx) {
    auto stroke_style = UIStrokeStyleColor({1, 1, 1, 1});
    ctx->set_stroke_style(&stroke_style);

    ctx->stroke_line({0, 0}, {0, m_size.y}, 2);
    ctx->stroke_line({0, 0}, {m_size.x, 0}, 2);
    ctx->stroke_line({m_size.x, m_size.y}, {0, m_size.y}, 2);
    ctx->stroke_line({m_size.x, m_size.y}, {m_size.x, 0}, 2);

    float delta = (float)m_size.x / (float)(m_array.size() + 1);

    auto fill_style_white = UIFillStyleColor({1, 1, 1, 1});
    auto fill_style_compared = UIFillStyleColor({1, 0, 0, 1});

    ctx->set_fill_style(&fill_style_white);

    for(int i = 0; i < m_array.size(); i++) {
        ctx->set_fill_style(&fill_style_white);
        if(&m_array[i] == m_collector.m_last_compared_a) {
            ctx->set_fill_style(&fill_style_compared);
            m_collector.m_last_compared_a = nullptr;
        }
        if(&m_array[i] == m_collector.m_last_compared_b) {
            ctx->set_fill_style(&fill_style_compared);
            m_collector.m_last_compared_b = nullptr;
        }
        ctx->fill_rect({delta * ((float) i + 0.5f), m_size.y - 10}, {delta - 1, -((float)m_size.y - 10) * (float)m_array[i].get_value() / (float)m_array_max});
    }

    if(m_sorting_is_waiting) {
        m_collector.continue_sorting();
        m_sorting_is_waiting = false;
    }
}

void AnimationView::async_task_entry(AnimationView* view, const std::function<void(void)>& func) {
    try {
        func();
        // TODO: The exception does not get caught sometimes
    } catch(SortingInterruptedException& ex) {
        return;
    }
}

AnimationView::~AnimationView() {
    perform_task_async([](){});
    m_thread->join();
    delete m_thread;
    m_thread = nullptr;
}
