//
// Created by Артем on 20.10.2021.
//

#include "dispatch_queue.hpp"

DispatchQueue DispatchQueue::main {};

void DispatchQueue::perform() {
    while(!tasks.empty()) {

        DispatchQueueTask task = *tasks.begin();
        uint64_t current_timestamp = get_current_timestamp();
        if(task.timestamp > current_timestamp) {
            std::this_thread::sleep_for(std::chrono::milliseconds(task.timestamp - current_timestamp));
        }

        tasks.erase(tasks.begin());

        task.task();

        auto timestamp_iter = task_timestamp_map.find(task.index);
        if(timestamp_iter != task_timestamp_map.end()) {
            task_timestamp_map.erase(timestamp_iter);
        }
    }
}

DispatchQueueTaskHandle DispatchQueue::push(DispatchQueueTask &&task) {
    task_counter++;
    task.index = task_counter;
    task_timestamp_map[task.index] = task.timestamp;
    tasks.insert(task);
    return task_counter;
}

bool DispatchQueue::cancel(DispatchQueueTaskHandle handle) {
    auto timestamp_iter = task_timestamp_map.find(handle);
    if(timestamp_iter == task_timestamp_map.end()) return false;
    uint64_t timestamp = timestamp_iter->second;
    task_timestamp_map.erase(timestamp_iter);

    DispatchQueueTask task {nullptr };
    task.timestamp = timestamp;
    task.index = handle;

    auto task_iter = tasks.find(task);
    if(task_iter == tasks.end()) return false;
    tasks.erase(task_iter);

    return true;
}

bool DispatchQueueTask::operator<(const DispatchQueueTask &other) const {
    if(timestamp < other.timestamp) return true;
    if(timestamp > other.timestamp) return false;
    return index < other.index;
}

DispatchQueueTask::DispatchQueueTask(const std::function<void(void)> &task) : task(task), timestamp(0) {}
DispatchQueueTask::DispatchQueueTask(const std::function<void(void)> &task, uint64_t delay) : task(task), timestamp(delay + get_current_timestamp()) {}

DispatchQueueTask::DispatchQueueTask(std::function<void(void)> &&task) : task(task), timestamp(0), index(0) {}
DispatchQueueTask::DispatchQueueTask(std::function<void(void)> &&task, uint64_t delay) : task(task), timestamp(delay + get_current_timestamp()) {}

