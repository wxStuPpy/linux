#include <iostream>

// 基类模板，用于递归继承的终止情况
template <typename... Args>
struct ParameterPackHolder {};

// 递归继承的类模板
template <typename T, typename... Args>
struct ParameterPackHolder<T, Args...> : public ParameterPackHolder<Args...> {
    T value;

    // 构造函数，初始化当前参数和递归调用基类的构造函数
    ParameterPackHolder(T val, Args... args) : ParameterPackHolder<Args...>(args...), value(val) {}

    // 用于获取当前参数值的函数
    T getValue() const {
        return value;
    }
};

// 辅助函数，用于打印参数包中的所有值
template <typename... Args>
void printParameterPack(const ParameterPackHolder<Args...>& holder) {
    if constexpr (sizeof...(Args) > 0) {
        std::cout << holder.getValue() << " ";
        // 递归调用，处理剩余的参数
        printParameterPack(static_cast<const ParameterPackHolder<Args...>&>(holder));
    } else {
        std::cout << std::endl;
    }
}

int main() {
    // 创建一个包含三个参数的 ParameterPackHolder 对象
    ParameterPackHolder<int, double, char> holder(42, 3.14, 'A');
    // 调用辅助函数打印参数包中的所有值
    printParameterPack(holder);
    return 0;
}