#pragma once

#include <thread>
#include <functional>
#include <map>
#include <set>

static uint64_t get_current_timestamp() {
    auto time = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
}

typedef uint64_t DispatchQueueTaskHandle;

struct DispatchQueueTask {
    std::function<void(void)> task;
    uint64_t timestamp;
    uint64_t index = 0;

    bool operator<(const DispatchQueueTask& other) const;

    DispatchQueueTask(const std::function<void(void)>& task);
    DispatchQueueTask(const std::function<void(void)>& task, uint64_t delay);

    DispatchQueueTask(std::function<void(void)>&& task);
    DispatchQueueTask(std::function<void(void)>&& task, uint64_t delay);
};

class DispatchQueue {
    uint64_t task_counter = 0;
    std::set<DispatchQueueTask> tasks {};
    std::map<DispatchQueueTaskHandle, uint64_t> task_timestamp_map {};

public:
    static DispatchQueue main;

    DispatchQueue() {}

    void perform();
    DispatchQueueTaskHandle push(DispatchQueueTask&& task);
    bool cancel(DispatchQueueTaskHandle handle);
};