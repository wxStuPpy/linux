在 C++ 模板编程中，模板特化是一种重要的技术，它允许针对特定的模板参数提供专门的实现。模板特化分为全特化和偏特化，下面分别介绍类模板和函数模板的全特化、偏特化，以及模板特化放置位置的建议。

### 类模板全特化
类模板全特化是指为模板的所有参数提供具体的类型，从而针对特定类型组合提供一个完全不同的类实现。

#### 语法
```cpp
template <>
class ClassName<SpecificType1, SpecificType2, ...> {
    // 特化后的类定义
};
```

#### 示例
```cpp
#include <iostream>

// 类模板
template <typename T>
class Container {
public:
    Container(T value) : data(value) {}
    void print() {
        std::cout << "General Container: " << data << std::endl;
    }
private:
    T data;
};

// 类模板全特化
template <>
class Container<int> {
public:
    Container(int value) : data(value) {}
    void print() {
        std::cout << "Specialized Container for int: " << data << std::endl;
    }
private:
    int data;
};

int main() {
    Container<double> c1(3.14);
    c1.print();

    Container<int> c2(42);
    c2.print();

    return 0;
}
```

### 类模板偏特化
类模板偏特化是指为模板的部分参数提供具体的类型，仍然保留部分参数作为模板参数。

#### 语法
```cpp
template <typename PartialType1, typename PartialType2, ...>
class ClassName<SpecificType1, PartialType1, SpecificType2, PartialType2, ...> {
    // 偏特化后的类定义
};
```

#### 示例
```cpp
#include <iostream>

// 类模板
template <typename T1, typename T2>
class Pair {
public:
    Pair(T1 first, T2 second) : first(first), second(second) {}
    void print() {
        std::cout << "General Pair: (" << first << ", " << second << ")" << std::endl;
    }
private:
    T1 first;
    T2 second;
};

// 类模板偏特化，固定第一个参数为 int
template <typename T>
class Pair<int, T> {
public:
    Pair(int first, T second) : first(first), second(second) {}
    void print() {
        std::cout << "Partial Specialized Pair with int first: (" << first << ", " << second << ")" << std::endl;
    }
private:
    int first;
    T second;
};

int main() {
    Pair<double, char> p1(3.14, 'A');
    p1.print();

    Pair<int, float> p2(42, 1.23f);
    p2.print();

    return 0;
}
```

### 函数模板全特化
函数模板全特化是指为函数模板的所有参数提供具体的类型，从而针对特定类型组合提供一个专门的函数实现。

#### 语法
```cpp
template <>
ReturnType FunctionName<SpecificType1, SpecificType2, ...>(ParameterList) {
    // 特化后的函数定义
}
```

#### 示例
```cpp
#include <iostream>

// 函数模板
template <typename T>
void print(T value) {
    std::cout << "General print: " << value << std::endl;
}

// 函数模板全特化
template <>
void print<int>(int value) {
    std::cout << "Specialized print for int: " << value << std::endl;
}

int main() {
    print(3.14);
    print(42);

    return 0;
}
```

### 函数模板偏特化
需要注意的是，C++ 标准中不支持函数模板的偏特化。如果需要针对部分参数进行特殊处理，可以使用函数重载来达到类似的效果。

### 模板特化放置位置建议
- **头文件（`.h` 或 `.hpp`）**
    - **全特化**：如果特化的代码是通用的，会在多个源文件中使用，建议将全特化定义放在头文件中。这样可以确保在不同的源文件中使用特化时，编译器能够找到对应的定义。
    - **偏特化**：类模板的偏特化通常也放在头文件中，因为偏特化也是通用的，可以在多个源文件中共享。
- **源文件（`.cpp`）**
    - 如果特化的代码只在某个源文件中使用，或者特化的实现依赖于特定源文件中的一些局部定义，可以将特化定义放在对应的源文件中。这样可以避免在头文件中暴露不必要的实现细节。

总之，模板特化的放置位置应根据特化代码的通用性和使用范围来决定，以保证代码的可维护性和可复用性。