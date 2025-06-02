#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

// 消息基类
struct Message {
    virtual ~Message() = default;
    virtual std::string name() const { return "Message"; }
};

// 具体消息类型
struct HelloMessage : public Message {
    std::string sender;
    HelloMessage(const std::string& s) : sender(s) {}
    std::string name() const override { return "HelloMessage"; }
};

struct ExitMessage : public Message {
    std::string name() const override { return "ExitMessage"; }
};

// Actor 基类
class Actor {
private:
    std::queue<std::shared_ptr<Message>> messages;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread worker;
    bool running = true;

    // 消息处理循环
    void processMessages() {
        // 循环处理消息
        while (running) {
            std::shared_ptr<Message> msg;
            {
                // 加锁保护消息队列
                std::unique_lock<std::mutex> lock(mtx);
                // 等待消息队列不为空或程序停止
                cv.wait(lock, [this] { return !messages.empty() || !running; });
                // 如果程序停止且消息队列为空，则跳出循环
                if (!running && messages.empty()) break;
                // 移动队列中的第一个消息到msg
                msg = std::move(messages.front());
                // 从队列中移除第一个消息
                messages.pop();
            }
            // 处理消息
            handleMessage(msg);
        }
    }

protected:
    // 子类需实现此方法处理消息
    virtual void handleMessage(const std::shared_ptr<Message>& msg) = 0;

public:
    Actor() : worker(&Actor::processMessages, this) {}
    
    virtual ~Actor() {
        send(std::make_shared<ExitMessage>());
        if (worker.joinable()) worker.join();
    }

    // 发送消息给当前 Actor
    void send(const std::shared_ptr<Message>& msg) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            messages.push(msg);
        }
        cv.notify_one();
    }
};

// 具体 Actor 实现
class PrintActor : public Actor {
protected:
    void handleMessage(const std::shared_ptr<Message>& msg) override {
        if (auto hello = std::dynamic_pointer_cast<HelloMessage>(msg)) {
            std::cout << "PrintActor: Hello from " << hello->sender << std::endl;
        } else if (std::dynamic_pointer_cast<ExitMessage>(msg)) {
            std::cout << "PrintActor: Exiting..." << std::endl;
        } else {
            std::cout << "PrintActor: Unknown message" << std::endl;
        }
    }
};

// Actor 系统（简化版）
class ActorSystem {
private:
    std::unordered_map<std::string, std::unique_ptr<Actor>> actors;

public:
    // 创建并注册 Actor
    template<typename T, typename... Args>
    //工厂模式创建Actor实例
    void createActor(const std::string& name, Args&&... args) {
        actors[name] = std::make_unique<T>(std::forward<Args>(args)...);
    }

    // 向 Actor 发送消息
    void send(const std::string& actorName, const std::shared_ptr<Message>& msg) {
        auto it = actors.find(actorName);
        if (it != actors.end()) {
            it->second->send(msg);
        } else {
            std::cerr << "Actor not found: " << actorName << std::endl;
        }
    }
};

// 使用示例
int main() {
    ActorSystem system;
    system.createActor<PrintActor>("printer");
    
    // 发送消息
    system.send("printer", std::make_shared<HelloMessage>("Main"));

    // 等待消息处理完成（实际应用中可能需要更复杂的同步）
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    return 0;
}