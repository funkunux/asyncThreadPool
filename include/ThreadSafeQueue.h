#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
public:
    typedef typename std::queue<T>::size_type size_type;
    typedef typename std::queue<T>::value_type value_type;
    ThreadSafeQueue(size_type max_size = 0) : max_size_(max_size) {};
    bool push(const value_type& item);
    void pop();
    value_type& front();
    value_type& back();
private:
    const size_type max_size_;
    std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable cond_;
};

template<typename T>
bool ThreadSafeQueue<T>::push(const typename ThreadSafeQueue<T>::value_type& item) {
    std::lock_guard<std::mutex> lock(mutex_);
    if(max_size_ && queue_.size() >= max_size_) {
        return false;
    }
    queue_.push(item);
    cond_.notify_one();
    return true;
}

template<typename T>
typename ThreadSafeQueue<T>::value_type& ThreadSafeQueue<T>::front() {
    std::unique_lock<std::mutex> lock(mutex_);
    while(queue_.empty()) {
        cond_.wait(lock);
    }

    return queue_.front();
}

template<typename T>
typename ThreadSafeQueue<T>::value_type& ThreadSafeQueue<T>::back() {
    std::unique_lock<std::mutex> lock(mutex_);
    while(queue_.empty()) {
        cond_.wait(lock);
    }

    return queue_.back();
}

template<typename T>
void ThreadSafeQueue<T>::pop() {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.pop();
}

