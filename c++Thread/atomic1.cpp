#include <iostream>
#include <mutex>
#include <atomic>
#include <thread>
#include <cassert>
#include <vector>

std::atomic_flag flag = ATOMIC_FLAG_INIT;

class SpinLock
{
public:
    void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire))
            ;
    }
    void unlock()
    {
        flag.clear(std::memory_order_release);
    }
    ~SpinLock()
    {
        unlock();
    }

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

void testLock()
{
    SpinLock lock;
    std::thread t1([&lock]()
                   {
        lock.lock();
        std::cout << "Thread 1 acquired lock" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        lock.unlock(); });

    std::cout << "testLock" << std::endl;

    std::thread t2([&lock]()
                   {
        lock.lock();
        std::cout << "Thread 2 acquired lock" << std::endl;
        lock.unlock(); });

    t1.join();
    t2.join();
    std::cout << "Done" << std::endl;
}


int main()
{
    // testLock();
    // memory_order_acquire_test::testAtomic1();
}