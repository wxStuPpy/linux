#include <iostream>
#include <thread>

class JoiningThread
{
private:
    std::thread _t;
public:
    JoiningThread() noexcept = default;
    explicit JoiningThread(std::thread t) : _t(std::move(t)) {}
    explicit JoiningThread(JoiningThread &&other) noexcept : _t(std::move(other._t))
    {
    }
    JoiningThread &operator=(JoiningThread &&other) noexcept
    {
        /*如果当前线程可汇合 先汇合再赋值*/
        if (this != &other && _t.joinable())
        {
            _t.join();
        }
        _t = std::move(other._t);
        return *this;
    }
    JoiningThread &operator=(JoiningThread other) noexcept
    {
        if (_t.joinable())
        {
            _t.join();
        }
        _t = std::move(other._t);
        return *this;
    }

    template <typename Callable, typename... Args>
    explicit JoiningThread(Callable &&func, Args &&...args) : _t(std::forward<Callable>(func),
                                                                 std::forward<Args>(args)...) {}
    ~JoiningThread()noexcept
    {
        if (_t.joinable())
        {
            _t.join();
        }
    }
};