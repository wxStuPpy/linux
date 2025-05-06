#include <iostream>

// 外部命名空间
namespace OuterNamespace {
    // 外部命名空间的变量
    int outerVariable = 10;

    // 内部命名空间
    namespace InnerNamespace {
        // 内部命名空间的变量
        int innerVariable = 20;

        // 内部命名空间的函数
        void innerFunction() {
            std::cout << "Inside InnerNamespace::innerFunction()" << std::endl;
        }
    }

    // 外部命名空间的函数
    void outerFunction() {
        std::cout << "Inside OuterNamespace::outerFunction()" << std::endl;
        // 调用内部命名空间的函数
        InnerNamespace::innerFunction();
    }
}

int main() {
    // 访问外部命名空间的变量
    std::cout << "OuterNamespace::outerVariable: " << OuterNamespace::outerVariable << std::endl;

    // 访问内部命名空间的变量
    std::cout << "OuterNamespace::InnerNamespace::innerVariable: " << OuterNamespace::InnerNamespace::innerVariable << std::endl;

    // 调用外部命名空间的函数
    OuterNamespace::outerFunction();

    // 使用 using 声明引入内部命名空间
    using namespace OuterNamespace::InnerNamespace;
    // 现在可以直接使用内部命名空间的成员
    innerFunction();

    return 0;
}