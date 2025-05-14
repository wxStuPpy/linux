#include <iostream>

// 递归终止函数
void printArgs() {
    std::cout << std::endl;
}

// 递归展开参数包
template <typename T, typename... Args>
void printArgs(T first, const Args&... args) {
    std::cout << first;
    if constexpr (sizeof...(args) > 0) {
        std::cout << ", ";
    }
    printArgs(args...);
}

int main() {
    printArgs(1, 2.5, "hello");
    return 0;
}