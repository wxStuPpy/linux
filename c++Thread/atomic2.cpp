#include <atomic>
#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

/*
memory_order_seq_cst 全局顺序一致性内存模型，保证操作的原子性以及操作的顺序。
但是存在一定的开销。
一个cpu写 其他cpu不能写也不能读，等该cpu写完后同步到memory给其他cpu。
*/
namespace memory_order_seq_cst_test
{
atomic<bool> x, y;
atomic<int> z;
void write_x_then_y()
{
    // 写操作 A 和 B 之间没有数据依赖，但是为了保证操作的顺序性，使用了 memory_order_seq_cst
    x.store(true, memory_order_seq_cst); // A
    y.store(true, memory_order_seq_cst); // B
}

void read_y_then_x()
{
    // 如果y被修改为true，那么x一定先于y被修改为true，z一定大于0
    while (!y.load(memory_order_seq_cst))
        ;                             // C
    if (x.load(memory_order_seq_cst)) // D
        ++z;
}

void test()
{
    x = false;
    y = false;
    z = 0;
    thread a(write_x_then_y);
    thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load() != 0);
    cout << "memory_order_seq_cst test passed" << endl;
}
}; // namespace memory_order_seq_cst_test

// 宽松内存序 同一线程 不同actomic 只保证原子性 可能不保证顺序执行
namespace memory_order_acquire_test
{
std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y()
{
    // 同一线程 不同actomic 可能不保证顺序执行
    x.store(true, std::memory_order_relaxed); // 1
    y.store(true, std::memory_order_relaxed); // 2
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_relaxed))
        ; // 3
    if (x.load(std::memory_order_relaxed))
    { // 4
        ++z;
    }
}

void test1()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load() != 0);
    std::cout << "Done" << std::endl;
}

void test2()
{
    std::atomic<int> a;
    std::vector<int> v1;
    std::vector<int> v2;
    std::thread t1([&a]() {
        for (int i = 0; i < 10; i += 2)
        {
            a.store(i, std::memory_order_relaxed);
        }
    });
    std::thread t2([&a]() {
        for (int i = 1; i < 10; i += 2)
        {
            a.store(i, std::memory_order_relaxed);
        }
    });
    std::thread t3([&a, &v1]() {
        for (int i = 0; i < 10; ++i)
        {
            v1.push_back(a.load(std::memory_order_relaxed));
        }
    });
    std::thread t4([&a, &v2]() {
        for (int i = 0; i < 10; ++i)
        {
            v2.push_back(a.load(std::memory_order_relaxed));
        }
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    for (int i = 0; i < 10; ++i)
    {
        std::cout << v1[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        std::cout << v2[i] << " ";
    }
    std::cout << "Done" << std::endl;
}
}; // namespace memory_order_acquire_test

namespace release_acquire
{
void test1()
{
    std::atomic<bool> x, y;
    std::atomic<int> z = 0;
    std::thread t1([&]() {
        // 虽然 x.store(relaxed) 不保证顺序，但 y.store(release) 强制将其之前的所有写操作刷新到内存。
        x.store(true, std::memory_order_relaxed); // 1
        y.store(true, std::memory_order_release); // 2
    });
    std::thread t2([&]() {
        while (!y.load(std::memory_order_acquire))
            ; // 3
        if (x.load(std::memory_order_relaxed))
        { // 4  x一定为true，z一定大于0
            ++z;
        }
    });
    t1.join();
    t2.join();
    assert(z != 0);
    cout << "Done" << endl;
}

void test2()
{
    /* 确保在该原子操作之前的所有内存访问（包括非原子操作），
    在另一个使用 memory_order_acquire 的线程中都能被正确观察到*/
    int data = 0;
    std::atomic<bool> ready;
    std::thread t1([&]() {
        data = 42;
        ready.store(true, memory_order_release);
    });
    std::thread t2([&]() {
        while (!ready.load(memory_order_acquire))
            ;
        assert(data == 42); // 确保data已经被写入
    });
    t1.join();
    t2.join();
    cout << "Done" << endl;
}

void test3()
{
    // 2和3只有一个会和4构成同步关系 5可能会失败 因为2和4可能不构成同步关系 因此不能保证x一定为1
    std::atomic<int> xd{0}, yd{0};
    std::atomic<int> zd;
    std::thread t1([&]() {
        xd.store(1, std::memory_order_release); // (1)
        yd.store(1, std::memory_order_release); //  (2)
    });

    std::thread t2([&]() {
        yd.store(2, std::memory_order_release); // (3)
    });

    std::thread t3([&]() {
        while (!yd.load(std::memory_order_acquire))
            ;                                            // （4）
        assert(xd.load(std::memory_order_acquire) == 1); // (5)
    });

    t1.join();
    t2.join();
    t3.join();
    cout << "Done" << endl;
}

void releaseSequence()
{
    std::vector<int> v;
    std::atomic<int> flag = 0;

    std::thread t1([&]() {
        v.push_back(1);
        flag.store(1, memory_order_release);
    });
    std::thread t2([&]() {
        int expected = 1;
        while (!flag.compare_exchange_weak(expected, 2, memory_order_acquire))
            ;
    });
    std::thread t3([&]() {
        while (flag.load(memory_order_acquire) != 2)
            ;
        assert(v[0] == 1);
    });
    t1.join();
    t2.join();
    t3.join();
    cout << "Done" << endl;
}
}; // namespace release_acquire

int main()
{
    // memory_order_seq_cst_test::test();
    // memory_order_acquire_test::test1();
    // memory_order_acquire_test::test2();
    // release_acquire::test1();
    // release_acquire::test2();
    // release_acquire::test3();
    release_acquire::releaseSequence();
    return 0;
}