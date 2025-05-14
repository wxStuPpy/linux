#include <iostream>

// 接收左值的函数
void func(int& x) {
    std::cout << "Lvalue reference: " << x << std::endl;
}

// 接收右值的函数
void func(int&& x) {
    std::cout << "Rvalue reference: " << x << std::endl;
}

// 完美转发函数模板
template<typename T>
void wrapper(T&& arg) {
    func(std::forward<T>(arg));
}

int main() {
    int a = 10;
    wrapper(a); // 传递左值
    wrapper(20); // 传递右值
    return 0;
}