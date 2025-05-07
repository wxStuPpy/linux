类成员函数指针和普通函数指针都是 C++ 中用于指向函数的工具，但它们在多个方面存在显著区别，以下为你详细介绍：

### 1. 指向的函数类型不同
- **普通函数指针**：指向全局函数或者类的静态成员函数。这些函数不依赖于特定的对象实例，没有隐含的 `this` 指针。
- **类成员函数指针**：指向类的非静态成员函数。非静态成员函数有一个隐含的 `this` 指针，它指向调用该函数的对象实例，因此成员函数的调用与特定对象相关联。

#### 示例代码
```cpp
#include <iostream>

// 全局函数
void globalFunction() {
    std::cout << "Global function called" << std::endl;
}

class MyClass {
public:
    // 静态成员函数
    static void staticFunction() {
        std::cout << "Static member function called" << std::endl;
    }

    // 非静态成员函数
    void nonStaticFunction() {
        std::cout << "Non - static member function called" << std::endl;
    }
};

int main() {
    // 普通函数指针指向全局函数
    void (*globalPtr)() = globalFunction;
    globalPtr();

    // 普通函数指针指向静态成员函数
    void (*staticPtr)() = &MyClass::staticFunction;
    staticPtr();

    // 类成员函数指针指向非静态成员函数
    void (MyClass::*nonStaticPtr)() = &MyClass::nonStaticFunction;
    MyClass obj;
    (obj.*nonStaticPtr)();

    return 0;
}
```

### 2. 定义和使用的语法不同
- **普通函数指针**：定义时只需要指定函数的返回类型和参数列表，使用时直接通过指针调用函数。
- **类成员函数指针**：定义时需要指定所属的类名，使用时必须结合具体的对象实例，通过 `.*`（用于对象）或 `->*`（用于对象指针）运算符来调用成员函数。

#### 示例代码
```cpp
// 普通函数指针定义和使用
void func(int a) {
    std::cout << "Value: " << a << std::endl;
}

void testRegularPointer() {
    void (*regularPtr)(int) = func;
    regularPtr(10);
}

// 类成员函数指针定义和使用
class AnotherClass {
public:
    void memberFunc(int a) {
        std::cout << "Member value: " << a << std::endl;
    }
};

void testMemberPointer() {
    void (AnotherClass::*memberPtr)(int) = &AnotherClass::memberFunc;
    AnotherClass obj;
    (obj.*memberPtr)(20);
}
```

### 3. 存储的信息不同
- **普通函数指针**：只存储函数的地址，因为全局函数和静态成员函数在内存中只有一份实例，调用时直接跳转到该地址执行。
- **类成员函数指针**：除了存储函数的地址外，还可能需要存储一些额外的信息（如偏移量），用于处理不同的继承和多态情况。这是因为不同的对象实例调用成员函数时，`this` 指针指向不同的对象，需要通过这些额外信息来正确定位函数的调用。

### 4. 调用的上下文不同
- **普通函数指针**：调用普通函数指针时，不需要特定的对象上下文，直接调用即可。
- **类成员函数指针**：调用类成员函数指针时，必须指定一个对象实例，因为成员函数的调用依赖于该对象的状态。

### 5. 类型兼容性不同
- **普通函数指针**：只要函数的返回类型和参数列表相同，不同的全局函数或静态成员函数可以使用同一个普通函数指针类型。
- **类成员函数指针**：不同类的成员函数指针类型是不兼容的，即使它们的返回类型和参数列表相同。因为成员函数指针还包含了所属类的信息。

综上所述，类成员函数指针和普通函数指针在指向的函数类型、定义和使用语法、存储信息、调用上下文以及类型兼容性等方面都存在明显的区别。在使用时需要根据具体的需求选择合适的指针类型。 