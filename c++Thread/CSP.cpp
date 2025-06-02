#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <functional>

// 通用通道模板
template<typename T>
class Channel {
private:
    std::queue<T> buffer;
    std::mutex mtx;
    std::condition_variable cv_send, cv_recv;
    bool closed = false;
    size_t capacity;

public:
    explicit Channel(size_t cap = 0) : capacity(cap) {}

    // 发送数据（阻塞直到通道有空间）
    bool send(const T& value) {
        std::unique_lock<std::mutex> lock(mtx);
        
        // 通道关闭后不能再发送
        if (closed) return false;
        
        // 有界通道且已满时等待
        if (capacity > 0) {
            cv_send.wait(lock, [this]{ return buffer.size() < capacity || closed; });
            if (closed) return false;
        }
        
        buffer.push(value);
        cv_recv.notify_one();  // 通知可能的接收者
        return true;
    }

    // 接收数据（阻塞直到有数据或通道关闭）
    bool receive(T& value) {
        std::unique_lock<std::mutex> lock(mtx);
        
        // 等待数据或通道关闭
        cv_recv.wait(lock, [this]{ return !buffer.empty() || closed; });
        
        // 通道已关闭且无数据
        if (buffer.empty() && closed) return false;
        
        value = buffer.front();
        buffer.pop();
        if (capacity > 0) {
            cv_send.notify_one();  // 通知可能的发送者（有界通道）
        }
        return true;
    }

    // 关闭通道
    void close() {
        std::lock_guard<std::mutex> lock(mtx);
        closed = true;
        cv_send.notify_all();  // 唤醒所有等待的发送者
        cv_recv.notify_all();  // 唤醒所有等待的接收者
    }

    // 判断通道是否关闭
    bool isClosed() const {
        std::lock_guard<std::mutex> lock(mtx);
        return closed;
    }
};

// 生产者函数
template<typename T>
void producer(Channel<T>& chan, int id, int count) {
    for (int i = 0; i < count; ++i) {
        T value = static_cast<T>(id * 100 + i);
        chan.send(value);
        std::cout << "Producer " << id << " sent: " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// 消费者函数
template<typename T>
void consumer(Channel<T>& chan, int id) {
    T value;
    while (chan.receive(value)) {
        std::cout << "Consumer " << id << " received: " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    std::cout << "Consumer " << id << " exiting..." << std::endl;
}

int main() {
    // 创建一个容量为 3 的有界通道
    Channel<int> chan(3);

    // 创建生产者和消费者线程
    std::thread p1(producer<int>, std::ref(chan), 1, 5);
    std::thread p2(producer<int>, std::ref(chan), 2, 5);
    std::thread c1(consumer<int>, std::ref(chan), 1);
    std::thread c2(consumer<int>, std::ref(chan), 2);

    // 等待生产者完成
    p1.join();
    p2.join();

    // 关闭通道，表示没有更多数据
    chan.close();

    // 等待消费者完成
    c1.join();
    c2.join();

    return 0;
}