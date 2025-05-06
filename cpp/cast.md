在 C++ 里，有四种类型的强制类型转换运算符，分别是 `static_cast`、`dynamic_cast`、`const_cast` 和 `reinterpret_cast`。下面为你详细介绍它们的用途和使用示例。

### 1. `static_cast`
`static_cast` 是最常用的类型转换运算符，它可以用于各种基本类型之间的转换、类层次结构中基类和派生类指针或引用的转换等。不过，它不会进行运行时类型检查，所以在使用时要确保转换是安全的。

#### 示例代码
```cpp
#include <iostream>

int main() {
    // 基本类型转换
    double d = 3.14;
    int i = static_cast<int>(d);
    std::cout << "Converted double " << d << " to int " << i << std::endl;

    // 类层次结构中的转换
    class Base {};
    class Derived : public Base {};

    Derived derived;
    Base* basePtr = static_cast<Base*>(&derived); // 派生类指针转换为基类指针

    return 0;
}
```

#### 代码解释
- 基本类型转换：把 `double` 类型的变量 `d` 转换为 `int` 类型的变量 `i`。
- 类层次结构中的转换：将派生类 `Derived` 的指针转换为基类 `Base` 的指针。

### 2. `dynamic_cast`
`dynamic_cast` 主要用于在类的继承层次结构中进行安全的向下转换（从基类指针或引用转换为派生类指针或引用）。它会在运行时进行类型检查，如果转换不安全，对于指针类型会返回 `nullptr`，对于引用类型会抛出 `std::bad_cast` 异常。

#### 示例代码
```cpp
#include <iostream>

class Base {
public:
    virtual void print() { std::cout << "Base" << std::endl; }
};

class Derived : public Base {
public:
    void print() override { std::cout << "Derived" << std::endl; }
};

int main() {
    Base* basePtr = new Derived();
    Derived* derivedPtr = dynamic_cast<Derived*>(basePtr);
    if (derivedPtr) {
        derivedPtr->print();
    }
    delete basePtr;
    return 0;
}
```

#### 代码解释
- 基类 `Base` 包含一个虚函数 `print`，派生类 `Derived` 对该函数进行了重写。
- 使用 `dynamic_cast` 将基类指针 `basePtr` 转换为派生类指针 `derivedPtr`，如果转换成功，则调用派生类的 `print` 函数。

### 3. `const_cast`
`const_cast` 主要用于去除或者添加 `const` 或 `volatile` 限定符。它只能用于改变对象的常量性或易变性，不能用于改变对象的类型。

#### 示例代码
```cpp
#include <iostream>

void modifyValue(int* num) {
    (*num)++;
}

int main() {
    const int num = 10;
    int* nonConstNum = const_cast<int*>(&num);
    modifyValue(nonConstNum);
    std::cout << "Modified value: " << *nonConstNum << std::endl;
    return 0;
}
```

#### 代码解释
- 定义了一个常量 `const int num`，使用 `const_cast` 去除其 `const` 限定符，得到一个普通的 `int` 指针 `nonConstNum`。
- 将 `nonConstNum` 传递给 `modifyValue` 函数，修改其值。

### 4. `reinterpret_cast`
`reinterpret_cast` 是一种比较危险的类型转换运算符，它可以将任意类型的指针或引用转换为其他类型的指针或引用，甚至可以将指针转换为整数类型。这种转换只是简单地重新解释二进制位，不进行任何类型检查，所以使用时要非常谨慎。

#### 示例代码
```cpp
#include <iostream>

int main() {
    int num = 10;
    int* numPtr = &num;
    long long address = reinterpret_cast<long long>(numPtr);
    std::cout << "Address of num: " << address << std::endl;
    return 0;
}
```

#### 代码解释
- 将 `int` 类型的指针 `numPtr` 转换为 `long long` 类型的整数 `address`，用于存储指针的地址。

### 总结
- `static_cast`：用于基本类型转换和类层次结构中基类和派生类指针或引用的转换，不进行运行时检查。
- `dynamic_cast`：用于类的继承层次结构中安全的向下转换，会进行运行时类型检查。
- `const_cast`：用于去除或添加 `const` 或 `volatile` 限定符。
- `reinterpret_cast`：用于任意类型的指针或引用之间的转换，只是简单重新解释二进制位，不进行类型检查。 