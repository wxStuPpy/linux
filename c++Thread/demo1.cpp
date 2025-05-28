#include<iostream>
#include<thread>
#include<chrono>

using namespace std;

// 线程数据结构，重载()运算符使其可作为线程函数
struct ThreadData {
    int _i;
    ThreadData(int i) : _i(i) {}
    // 线程执行体：循环3次，输出_i并自增，每次间隔1秒
    void operator()() {
        for(int i = 0; i < 3; ++i) {
            cout << _i++ << endl;
            this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

// 线程守卫类：使用RAII机制自动管理线程生命周期
class ThreadGuard {
public:
    // 通过移动构造函数获取线程所有权
    explicit ThreadGuard(std::thread&& th) : t(std::move(th)) {
        if (!t.joinable()) throw std::invalid_argument("Thread not joinable");
    }
    // 析构时自动join线程，确保资源释放
    ~ThreadGuard() {
        if (t.joinable()) t.join();
    }
private:
    std::thread t; // 持有线程对象（值语义）
};

#if 0
threadGuard createGuard() {
    thread t(someFunction);
    threadGuard tg(t); // 危险！t的生命周期仅限于函数内 原来引用函数
    return tg; // 返回后，t已销毁，tg._t变为悬空引用
} // t在函数结束时析构
#endif

// 返回线程对象的函数（演示移动语义）
std::thread f() {
    return std::thread([](){
        cout << "Hello from another thread" << endl;
    });
}

// 引用传递测试函数
void refFun(int& i) {
    ++i;
    cout << i << endl;
}

int main() {
    // 创建线程并通过ThreadGuard管理
    ThreadData t1(5);
    thread t(t1);
    ThreadGuard tg1(std::move(t)); // 转移线程所有权到守卫

    // 带引用参数的线程
    int x = 0;
    thread t2(refFun, std::ref(x)); // 使用std::ref传递引用
    ThreadGuard tg2(std::move(t2)); // 转移所有权

    // 通过函数返回值创建线程
    auto t3 = f();
    ThreadGuard tg3(std::move(t3)); // 转移所有权

    // 所有线程在对应的ThreadGuard析构时自动join
    return 0;
}