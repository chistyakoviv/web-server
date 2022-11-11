#include "thread_pool.h"

ThreadPool::ThreadPool(uint8_t threadsNum) : running(true) {
    auto fn = [&]() {
        while (running) {
            std::unique_lock<std::mutex> lock(mutex);
            while (queue.Empty()) {
                cv.wait(lock);
            }
            std::function<void()> fn = queue.Pop();
            fn();
        }
    };
    for (uint8_t i = 0; i < threadsNum; i++) {
        threads.push_back(std::thread(fn));
    }
}

void ThreadPool::Push(std::function<void()> fn) {
    std::unique_lock<std::mutex> lock(mutex);
    queue.Push(fn);
    cv.notify_one();
}