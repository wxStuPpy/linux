#include <iostream>
#include <utility> // for std::move

class A
{
public:
    // 默认构造函数
    A() : p(new int(10))
    {
        std::cout << "Default constructor called" << std::endl;
    }

    // 拷贝构造函数（深拷贝）
    A(const A &obj) : p(obj.p ? new int(*obj.p) : nullptr)
    {
        std::cout << "Copy constructor called" << std::endl;
    }

    // 移动构造函数（资源转移）noexcept通知标准库不抛出任何异常
    A(A &&obj) noexcept : p(obj.p)
    {
        obj.p = nullptr;
        std::cout << "Move constructor called" << std::endl;
    }

    A &operator=(A &&obj) noexcept
    {
        std::cout << "Move = called" << std::endl;
        if (this == &obj)
        {
            return *this;
        }
        delete p; /*先删去自己的内存*/
        p = obj.p;
        obj.p = nullptr;
        return *this;
    }

    // 析构函数
    ~A()
    {
        delete p; // 安全释放（delete nullptr 无害）
        std::cout << "Destructor called" << std::endl;
    }

    static A getA()
    {
        A a;
        return a; // 可能触发 RVO 或移动构造
    }

    int *p;
};

int main()
{
    A a1;                // 默认构造
    A a2(a1);            // 拷贝构造
    A a3(std::move(a1)); // 移动构造（正确转移资源）
    std::cout << "a1.p: " << a1.p << ", a3.p: " << *a3.p << std::endl;

    A a4 = A::getA(); // 可能触发 RVO
    A a5;
    a5 = std::move(a4);

    return 0;
}