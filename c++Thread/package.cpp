#include <iostream>
#include <future>
#include <thread>
#include <stdexcept>
#include <vector>

// 示例任务函数
int myTask(int a) {
    if (a < 0) throw std::invalid_argument("Input must be non-negative");
    return a * 2;
}

// 改进的 packaged_task 示例（使用 std::async 替代手动线程管理）
void usePackagedTask() {
    std::cout << "\n=== Using std::async (替代 packaged_task) ===" << std::endl;
    
    // 使用 std::async 自动管理线程和结果
    auto future = std::async(std::launch::async, myTask, 10);
    
    // 主线程可以继续执行其他任务...
    
    try {
        std::cout << "Task result: " << future.get() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

// 改进的 promise 示例（分离线程，异步获取结果）
void usePromise() {
    std::cout << "\n=== Using std::promise ===" << std::endl;
    
    std::promise<int> p;
    std::future<int> f = p.get_future();
    
    // 使用 lambda 捕获 promise 引用，在线程中设置值
    std::thread([&p]() {
        try {
            // 模拟耗时操作
            std::this_thread::sleep_for(std::chrono::seconds(1));
            p.set_value(42);  // 设置结果
            std::cout << "Promise value set." << std::endl;
        } catch (...) {
            // 异常处理：通过 promise 传递异常
            p.set_exception(std::current_exception());
        }
    }).detach();  // 分离线程，让其自行结束
    
    // 主线程可以继续执行其他任务...
    
    // 当需要结果时再阻塞（若线程未完成，get() 会等待）
    try {
        std::cout << "Future value: " << f.get() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

// 演示异常处理
void demonstrateException() {
    std::cout << "\n=== Demonstrating Exception Handling ===" << std::endl;
    
    auto future = std::async(std::launch::async, myTask, -5);  // 传递非法参数
    
    try {
        std::cout << "Result: " << future.get() << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Caught invalid_argument: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }
}

bool stop=false;
std::vector<std::thread> threads;
std::mutex mtx;
std::condition_variable cv;
template<typename F, typename... Args>
auto commit(F&&f,Args&&...args)->std::future<decltype(f(args...))>{
    using RetType=decltype(f(args...));
    if(stop)
    return std::future<RetType>();
    auto task=std::make_shared<std::packaged_task<RetType()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<RetType> res=task->get_future();
    {
        std::lock_guard<std::mutex> lock(mtx);
        threads.emplace_back([task](){(*task)();});
    }
    cv.notify_one();
    return res;
}

int main() {
    usePackagedTask();
    usePromise();
    demonstrateException();
    
    return 0;
}