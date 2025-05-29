#include <mutex>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace std;

//c++14
void test1(){
    mutex mtx1;
    mutex mtx2;
    // 使用 std::lock 同时锁定多个互斥锁，避免死锁
    std::lock(mtx1, mtx2);
    
    // 使用 lock_guard 管理锁的生命周期，确保异常安全
    {
        lock_guard<mutex> lk1(mtx1, adopt_lock);
        lock_guard<mutex> lk2(mtx2, adopt_lock);
        
        // 受保护的代码区域
        cout << "Hello World" << endl;
    }  // 离开作用域时，lk2 和 lk1 按逆序释放
}

//c++17 使用 scoped_lock 管理多个锁的生命周期，简化代码
void test2() {
    mutex mtx1, mtx2;
    // 非关键代码...
    {
        scoped_lock lk(mtx1, mtx2);
        // 关键代码（需要保护）
        cout << "Hello World" << endl;
    }
    // 非关键代码...
}

class HierarchicalMutex {
private:
    std::mutex internal_mutex;
    const unsigned long hierarchy_value;  // 当前锁的层级值
    unsigned long previous_hierarchy_value;  // 线程之前持有的锁的层级值
    
    // 线程局部变量：记录当前线程的层级值
    //thread_local是C++11的特性，用于声明线程局部变量 每个线程都有自己的副本 不会与其他线程共享
    static thread_local unsigned long this_thread_hierarchy_value;


    void check_for_hierarchy_violation() {
        // 检查是否违反层级顺序（获取了比当前层级低的锁）
        if (this_thread_hierarchy_value <= hierarchy_value) {
            throw std::logic_error("Lock hierarchy violated!");
        }
    }

    void update_hierarchy_value() {
        previous_hierarchy_value = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value;
    }

public:
    explicit HierarchicalMutex(unsigned long value)
        : hierarchy_value(value), previous_hierarchy_value(0) {}

    void lock() {
        check_for_hierarchy_violation();
        internal_mutex.lock();
        update_hierarchy_value();
    }

    void unlock() {
        // 恢复线程的前一个层级值
        this_thread_hierarchy_value = previous_hierarchy_value;
        internal_mutex.unlock();
    }

    bool try_lock() {
        check_for_hierarchy_violation();
        if (!internal_mutex.try_lock()) return false;
        update_hierarchy_value();
        return true;
    }
};

// 初始化线程局部变量
thread_local unsigned long HierarchicalMutex::this_thread_hierarchy_value(ULONG_MAX);

// 使用示例
HierarchicalMutex high_level_mutex(1000);
HierarchicalMutex low_level_mutex(500);

void thread_function() {
    std::lock_guard<HierarchicalMutex> high_lock(high_level_mutex);
    std::lock_guard<HierarchicalMutex> low_lock(low_level_mutex);
    std::cout << "Locked both mutexes in correct order" << std::endl;
}

void thread_function_violation() {
    // 错误示例：违反层级顺序（先获取低层级锁，再获取高层级锁）
    std::lock_guard<HierarchicalMutex> low_lock(low_level_mutex);
    std::lock_guard<HierarchicalMutex> high_lock(high_level_mutex);  // 抛出异常
}

int main() {
    test1();
    test2();
    return 0;
}