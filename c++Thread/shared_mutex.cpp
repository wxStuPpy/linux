#include <shared_mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <chrono>
#include <mutex>

class ThreadSafeCounter {
private:
    mutable std::shared_mutex mutex_;  // 读写锁
    int count_ = 0;

public:
    // 读取操作（允许多线程并发）
    int get() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);  // 共享锁（读锁）
        return count_;
    }

    // 写入操作（互斥执行）
    void increment() {
        std::unique_lock<std::shared_mutex> lock(mutex_);  // 排他锁（写锁）
        ++count_;
    }

    // 重置操作（互斥执行）
    void reset() {
        std::unique_lock<std::shared_mutex> lock(mutex_);  // 排他锁
        count_ = 0;
    }
};

int main() {
    ThreadSafeCounter counter;
    const int num_readers = 10;
    const int num_writers = 2;
    const int iterations = 1000;

    // 启动多个读线程
    std::vector<std::thread> readers;
    for (int i = 0; i < num_readers; ++i) {
        readers.emplace_back([&counter, iterations] {
            for (int j = 0; j < iterations; ++j) {
                std::cout << "Reader " << std::this_thread::get_id() 
                          << " read: " << counter.get() << std::endl;
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
        });
    }

    // 启动多个写线程
    std::vector<std::thread> writers;
    for (int i = 0; i < num_writers; ++i) {
        writers.emplace_back([&counter, iterations] {
            for (int j = 0; j < iterations; ++j) {
                counter.increment();
                std::cout << "Writer " << std::this_thread::get_id() 
                          << " incremented to: " << counter.get() << std::endl;
                std::this_thread::sleep_for(std::chrono::microseconds(5));
            }
        });
    }

    // 等待所有线程完成
    for (auto& t : readers) t.join();
    for (auto& t : writers) t.join();

    std::cout << "Final count: " << counter.get() << std::endl;
    return 0;
}