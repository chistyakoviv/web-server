#pragma once

#include "queue.h"

#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>

class ThreadPool {
public:
    ThreadPool() = delete;
    ThreadPool(uint8_t threadsNum);

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;

    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    ~ThreadPool() = default;

    void Push(std::function<void()> fn);
private:
    std::vector<std::thread> threads;
    Queue<std::function<void()>> queue;
    std::mutex mutex;
    std::condition_variable cv;
    bool running;
};