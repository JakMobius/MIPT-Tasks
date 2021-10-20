#pragma once

#include <vector>
#include <queue>
#include <functional>

class DispatchQueue {

    std::queue<std::function<void(void)>> tasks {};
public:
    static DispatchQueue main;

    DispatchQueue() {}

    bool empty() {
        return tasks.empty();
    }

    void perform() {
        int count = tasks.size();

        for(int i = 0; i < count; i++) {
            auto task = tasks.front();
            tasks.pop();
            task();
        }
    }

    void push(std::function<void(void)> functor) {
        tasks.push(functor);
    }
};