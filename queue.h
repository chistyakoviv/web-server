#include <queue>
#include <mutex>

template<typename T>
class Queue {
public:
    Queue() = default;
    ~Queue() = default;

    void Push(const T& elem) {
        mutex.lock();
        queue.push(elem);
        mutex.unlock();
    }

    T Pop() {
        mutex.lock();
        T res = queue.front();
        queue.pop();
        mutex.unlock();
        return res;
    }

    bool Empty() {
        mutex.lock();
        bool res = queue.empty();
        mutex.unlock();
        return res;
    }
private:
    std::queue<T> queue;
    std::mutex mutex;
};