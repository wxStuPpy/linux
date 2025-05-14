`std::shared_ptr` 和 `std::unique_ptr` 都是 C++ 标准库 `<memory>` 头文件中用于管理动态分配对象的智能指针，它们的主要区别体现在所有权和资源管理方式上，以下为你详细介绍：

### 1. 所有权特性
- **`std::unique_ptr`**
    - 具有独占所有权，即同一时间内，一个 `std::unique_ptr` 只能有一个实例指向某个对象。当这个 `std::unique_ptr` 被销毁或者指向另一个对象时，它所管理的对象也会被自动删除。
    - 不能进行拷贝构造或拷贝赋值操作，因为这会破坏独占所有权的规则，但可以通过 `std::move` 进行所有权的转移。
- **`std::shared_ptr`**
    - 具备共享所有权，多个 `std::shared_ptr` 实例可以同时指向同一个对象。它内部维护了一个引用计数，记录有多少个 `std::shared_ptr` 指向该对象。
    - 支持拷贝构造和拷贝赋值操作，每次进行拷贝时，引用计数会增加；当 `std::shared_ptr` 被销毁或者指向其他对象时，引用计数会减少。当引用计数变为 0 时，对象会被自动删除。

### 2. 资源管理方式
- **`std::unique_ptr`**
    - 资源管理简单直接，由于独占所有权，在 `std::unique_ptr` 生命周期结束时，所管理的对象会被立即释放。
    - 适合管理独占性的资源，如文件句柄、网络连接等。
- **`std::shared_ptr`**
    - 资源管理相对复杂，需要维护引用计数。引用计数的维护会带来一定的性能开销，但可以更灵活地共享资源。
    - 适用于多个地方需要访问同一个对象，并且需要确保在所有访问者都不再使用该对象时才释放资源的场景。

### 3. 性能方面
- **`std::unique_ptr`**
    - 由于不需要维护引用计数，`std::unique_ptr` 的开销较小，性能更高。它的操作通常是常数时间复杂度。
- **`std::shared_ptr`**
    - 引用计数的维护会带来额外的开销，包括内存分配和原子操作。在频繁进行拷贝和销毁操作时，性能会受到一定影响。

### 4. 使用示例
下面是 `std::unique_ptr` 和 `std::shared_ptr` 的简单使用示例：
```cpp
#include <iostream>
#include <memory>

class MyClass {
public:
    MyClass() { std::cout << "MyClass constructor" << std::endl; }
    ~MyClass() { std::cout << "MyClass destructor" << std::endl; }
};

void uniquePtrExample() {
    // 创建一个 std::unique_ptr
    std::unique_ptr<MyClass> uniquePtr = std::make_unique<MyClass>();
    // 所有权转移
    std::unique_ptr<MyClass> anotherUniquePtr = std::move(uniquePtr); 
    // 此时 uniquePtr 为空
    if (!uniquePtr) {
        std::cout << "uniquePtr is empty" << std::endl;
    }
}

void sharedPtrExample() {
    // 创建一个 std::shared_ptr
    std::shared_ptr<MyClass> sharedPtr1 = std::make_shared<MyClass>();
    std::cout << "SharedPtr1 use count: " << sharedPtr1.use_count() << std::endl;
    // 拷贝构造
    std::shared_ptr<MyClass> sharedPtr2 = sharedPtr1; 
    std::cout << "SharedPtr1 use count: " << sharedPtr1.use_count() << std::endl;
    std::cout << "SharedPtr2 use count: " << sharedPtr2.use_count() << std::endl;
}

int main() {
    std::cout << "UniquePtr example:" << std::endl;
    uniquePtrExample();
    std::cout << "\nSharedPtr example:" << std::endl;
    sharedPtrExample();
    return 0;
}
```
### 总结
- 如果你需要独占资源，避免资源被意外共享，并且追求更高的性能，那么选择 `std::unique_ptr`。
- 如果你需要多个地方共享同一个对象，并且需要自动管理对象的生命周期，那么选择 `std::shared_ptr`。 

下面我会为你详细介绍 `std::unique_ptr` 和 `std::shared_ptr` 如何管理数组，同时说明怎样使用自定义删除器。

### 1. `std::unique_ptr` 管理数组及使用删除器
`std::unique_ptr` 能直接管理数组，并且可以使用自定义删除器。对于数组管理，标准库提供了特化版本的 `std::unique_ptr` 来处理。

```cpp
#include <iostream>
#include <memory>

// 自定义删除器，用于删除数组
struct ArrayDeleter {
    void operator()(int* ptr) const {
        std::cout << "Deleting array..." << std::endl;
        delete[] ptr;
    }
};

void uniquePtrArrayExample() {
    // 使用自定义删除器管理数组
    std::unique_ptr<int[], ArrayDeleter> uniqueArray(new int[5]);
    for (int i = 0; i < 5; ++i) {
        uniqueArray[i] = i;
    }
    for (int i = 0; i < 5; ++i) {
        std::cout << uniqueArray[i] << " ";
    }
    std::cout << std::endl;
}

```

### 2. `std::shared_ptr` 管理数组及使用删除器
`std::shared_ptr` 本身没有对数组管理进行特化，但可以通过自定义删除器来管理数组。

```cpp
// 自定义删除器，用于删除数组
struct ArrayDeleterShared {
    void operator()(int* ptr) const {
        std::cout << "Deleting shared array..." << std::endl;
        delete[] ptr;
    }
};

void sharedPtrArrayExample() {
    // 使用自定义删除器管理数组
    std::shared_ptr<int> sharedArray(new int[5], ArrayDeleterShared());
    for (int i = 0; i < 5; ++i) {
        *(sharedArray.get() + i) = i;
    }
    for (int i = 0; i < 5; ++i) {
        std::cout << *(sharedArray.get() + i) << " ";
    }
    std::cout << std::endl;
}
```



