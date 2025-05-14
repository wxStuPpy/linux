#include <iostream>
#include <thread>
#include <memory>
#include <functional>
#include <mutex>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <queue>
#include <condition_variable>
#include <vector>
#include <future>
#include <atomic>

// 1.创建thread进程
void fun01(std::string msg)
{
    std::cout << msg << std::endl;
    return;
}
void test01()
{
    std::thread thread1(fun01, "hello world"); // 给线程中的函数传参是在后面接着写
    // thread1.detach();//即使主线程结束 子线程也会在后台运行(用的不多)

    // 有些线程不能使用jion/detach 使用之前需要判断
    bool isJoin = thread1.joinable();
    if (isJoin)         // join方法会阻塞
        thread1.join(); // 子线程会阻塞主线程 主线程会一直等待到所有子线程执行完毕
}

// 2.引用的传递
void fun02(int &x)
{
    x++;
}
void test02()
{
    int a = 0;
    std::thread thread1(fun02, std::ref(a));
    bool isJoin = thread1.joinable();
    if (isJoin)
        thread1.join();
    std::cout << a << std::endl;
}
// 3.变量的生命周期(注意main函数中的内容)
std::thread g_t;
int g_b = 0;
void test03()
{
    g_b = 0;
    g_t = std::thread(fun02, std::ref(g_b));
    std::cout << "test中g_b=" << g_b << std::endl;
}
// 3.绑定类内成员函数或静态成员函数或私有成员函数
class A
{
    // 友元
    friend void test04();

public:
    A() { data = 0; }
    A(int x) { data = x; }
    void funA()
    {
        std::cout << "hello" << std::endl;
    }
    static void funAA()
    {
        std::cout << "world" << std::endl;
    }

private:
    void privateFunA()
    {
        std::cout << "private:" << data << std::endl;
    }
    int data;
};
void test04()
{
    std::shared_ptr<A> ptr = std::make_shared<A>(1);
    // std::thread tt(&A::funA,ptr);bind更安全
    // 类普通成员函数
    std::thread t(std::bind(&A::funA, ptr));
    if (t.joinable())
        t.join();
    // 类静态成员函数
    std::thread tt(A::funAA);
    if (tt.joinable())
        tt.join();
    // 类私有成员函数
    std::thread ttt(std::bind(&A::privateFunA, ptr));
    if (ttt.joinable())
        ttt.join();
}

// 5.互斥量解决多线程共享问题(如果多个线程共享一个数据 如果其中有一个线程修改了数据 就会出错)
int g_a = 0;
int g_c = 0;
std::mutex mut1;
void fun05()
{
    for (int i = 0; i < 20000; ++i)
        g_a++;
    for (int i = 0; i < 20000; ++i)
    {
        mut1.lock(); // 加锁
        g_c++;
        mut1.unlock(); // 解锁
    }
}
void test05()
{
    // 重置变量
    g_a = g_c = 0;

    std::thread t1(fun05);
    std::thread t2(fun05);
    if (t1.joinable())
        t1.join();
    if (t2.joinable())
        t2.join();
    std::cout << "g_a=" << g_a << std::endl; // 可能存在进程t1 t2同时操作g_a的情况 导致g_a的值偏小
    std::cout << "g_c=" << g_c << std::endl; // g_c被加锁 只能允许在同一时刻一个线程去操作g_c
}
// 6.lock_guard与unique_lock
std::timed_mutex mut2;
void fun06()
{
    for (int i = 0; i < 2; ++i)
    {
        // std::lock_guard<std::mutex>lock(mut2);//等效fun05()中加锁解锁
        std::unique_lock<std::timed_mutex> lock(mut2, std::defer_lock); // 传入参数defer_lock代表不自动加锁 需要程序猿手动加锁
        // 手动加锁可以提供多种加锁方式
        if (lock.try_lock_for(std::chrono::seconds(1))) // 加锁时只等待1钟 不行就不加锁
        {
            std::this_thread::sleep_for(std::chrono::seconds(2)); // 该线程休眠2秒钟
            g_a++;
        }
    }
}
void test06()
{
    g_a = 0;
    std::thread t1(fun06);
    std::thread t2(fun06);
    if (t1.joinable())
        t1.join();
    if (t2.joinable())
        t2.join();
    std::cout << "g_a=" << g_a << std::endl;
}

// 7.call_once与其使用场景(单例模式)
// class Log;
// static Log* log = nullptr;
// static std::once_flag once;
// class Log {
// public:
//	Log(const Log&) = delete;
//	Log& operator=(const Log&) = delete;
//
//	static Log& GetInstance() {
//		/*static Log log;
//		return log;*/ //饿汉模式
//		//或者
//		std::call_once(once, init);//call_once之后创建一次对象 防止多个进程创建多个单例对象 该函数不能再main中调用
//		return *log;//懒汉模式
//	}
//	static void init() {
//		if (!log)
//			log = new Log;
//	}
//	void printLog() {
//		auto now = std::chrono::system_clock::now();
//		std::time_t time_now = std::chrono::system_clock::to_time_t(now);
//		std::tm time_info;
//		if (localtime_s(&time_info, &time_now) == 0) {
//			std::cout << std::put_time(&time_info, "%c") << " error" << std::endl;
//		}
//		else {
//			std::cerr << "Error converting time." << std::endl;
//		}
//	}
// private:
//	Log() {}
// };
//
// void test07() {
//	Log::GetInstance().printLog();
// }

// 8.condition_variable
std::queue<int> g_queue;
std::condition_variable g_cv;
std::mutex mux3;

void Producer()
{
    for (int i = 0; i < 10; ++i)
    {
        {
            std::unique_lock<std::mutex> lock(mux3);
            g_queue.push(i);
            // 加任务时通知消费者来取任务
            g_cv.notify_one();
            std::cout << "task:" << i << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}
void Consumer()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mux3);
        // 如果队列为空 那么消费者就要等待生产者的任务
        g_cv.wait(lock, [&]()
                  { return !g_queue.empty(); });
        int value = g_queue.front();
        g_queue.pop();

        std::cout << "value:" << value << std::endl;
    }
}
void test08()
{
    std::thread t1(Producer);
    std::thread t2(Consumer);
    t1.join();
    t2.join();
}

// 9.c++11实现线程池
class ThreadPool
{
public:
    // 构造函数，接收线程数量作为参数
    ThreadPool(int numThreads) : stop(false)
    {
        // 创建指定数量的工作线程
        for (int i = 0; i < numThreads; ++i)
        {
            // 将线程添加到线程向量中
            threads.emplace_back([this]()
                                 {
				// 线程循环
				while (1) {
					// 加锁，保护任务队列的操作
					std::unique_lock<std::mutex> lock(mtx);
					// 等待任务队列不为空或线程池停止
					c_v.wait(lock, [this]() {
						return!tasks.empty() || stop;
						});
					// 如果线程池停止且任务队列为空，线程结束
					if (stop && tasks.empty())
						return;
					// 从任务队列中取出一个任务
					std::function<void()> task = std::move(tasks.front());
					tasks.pop();
					// 解锁，以便其他线程可以操作任务队列
					lock.unlock();
					// 执行任务
					task();
				} });
        }
    }

    // 析构函数，用于停止线程池
    ~ThreadPool()
    {
        {
            // 加锁，设置停止标志
            std::unique_lock<std::mutex> lock(mtx);
            stop = true;
        }
        // 通知所有线程停止
        c_v.notify_all();

        // 等待所有线程完成
        for (auto &t : threads)
            if (t.joinable())
                t.join();
    }

    // 向线程池添加任务的模板函数
    template <class T, class... Args>
    void Enqueue(T &&t, Args &&...args)
    {
        // 将任务及其参数绑定为一个可调用函数
        std::function<void()> task = std::bind(std::forward<T>(t), std::forward<Args>(args)...);
        {
            // 加锁，将任务添加到任务队列中
            std::unique_lock<std::mutex> lock(mtx);
            tasks.emplace(std::move(task));
        }
        // 通知一个线程有新任务
        c_v.notify_one();
    }

private:
    // 存储线程的向量
    std::vector<std::thread> threads;
    // 存储任务的队列
    std::queue<std::function<void()>> tasks;

    // 互斥锁，用于保护任务队列的操作
    std::mutex mtx;
    // 条件变量，用于线程间的通信
    std::condition_variable c_v;

    // 线程池停止标志
    bool stop;
};
void test09()
{
    ThreadPool pool(4);
    for (int i = 0; i < 10; ++i)
    {
        pool.Enqueue([i]()
                     {
			std::cout << "task" << i << " is running" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << "task" << i << " is done" << std::endl; });
    }
}

// 10.async
int fun10()
{
    int i = 0;
    for (; i < 1000; ++i)
    {
        i++;
    }
    return i;
}
void test10()
{
    std::future<int> future_res = std::async(std::launch::async, fun10); // 可以理解为在后台已经开始调用fun10 不阻塞
    std::cout << future_res.get() << std::endl;
    std::cout << fun10() << std::endl;
}

// 11.package_task  //可移动对象 需要move执行
void test11()
{
    std::packaged_task<int()> task(fun10);
    auto future_result = task.get_future();
    std::thread t(std::move(task));
    if (t.joinable())
        t.join(); // 手动开启线程 让fun10在后台运行
    std::cout << future_result.get() << std::endl;
}

// 12.promise
//  函数 fun12 接收一个 std::promise<int> 对象作为参数
//  它将一个值 1 设置到这个 promise 中
void fun12(std::promise<int> f)
{
    // 设置 promise 的值为 1
    f.set_value(1);
}

// 函数 test12 用于测试使用 promise 和 future 进行线程间通信
void test12()
{
    // 创建一个 std::promise<int> 对象
    std::promise<int> f;
    // 从 promise 获取一个 future 对象，用于获取异步操作的结果
    auto future_result = f.get_future();
    // 创建一个新的线程，将 fun12 函数和 promise 对象作为参数传递给线程
    // 使用 std::move 将 f 转移给线程，因为 promise 是不能复制的
    std::thread t(fun12, std::move(f));
    // 检查线程是否可 join
    if (t.joinable())
    {
        // 等待线程完成
        t.join();
        // 从 future 中获取结果，这里会阻塞直到结果可用
        std::cout << future_result.get() << std::endl;
    }
}

// 13.原子操作atomic 比手动加锁效率更高
std::atomic<int> data_atomic = 0;
void fun13()
{
    for (int i = 0; i < 20000; ++i)
    {
        // 原子递增操作，确保在多线程环境下的操作安全 与加锁方式一直
        data_atomic++;
    }
}
void test13()
{
    std::thread t1(fun13);
    std::thread t2(fun13);
    if (t1.joinable())
        t1.join();
    if (t2.joinable())
        t2.join();
    // loda store 可以保证操作都是原子性的
    std::cout << "data_atomic=" << data_atomic.load() << std::endl;
    data_atomic.store(0); // 修改值
    std::cout << "data_atomic=" << data_atomic.load() << std::endl;
}
int main()
{
    //{3
    //	test03();//线程还没执行完 g_b的值可能没有被修改 如果b是局部变量 此时g_b已经被释放 join会发生错误
    //	t.join();//因此 意味着jion也要放到一个函数内部 不要独立拿出来(这里提供一个错误的案例)
    //	std::cout << "main中g_b=" << g_b << std::endl;//线程执行完毕 g_b已经被修改
    //}
    return 0;
}
